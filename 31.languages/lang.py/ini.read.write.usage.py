#!/usr/bin/python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jul 13 10:50:54 2021
@note: 大多数 linux 发行版默认不带 ConfigObj 需要自行安装
       debian ubuntu 的安装方法 apt install python3-configobj
"""
import os, sys
from configobj import ConfigObj
from shutil import copyfile

class ExpandIfpcos:
    ifpath = None
    ofpath = None
    factor = None
    config = None
    def __init__(self, _ifpath, _ofpath, _factor):
        # 传递参数
        self.ifpath = _ifpath
        self.ofpath = _ofpath
        self.factor = _factor

    def doCheck(self):
        # 输入路径不存在
        if os.path.exists(self.ifpath) == False:
            self.__die("input [%s] is not exists"%(self.ifpath))
        # 输入路径非文件
        if os.path.isfile(self.ifpath) == False:
            self.__die("input [%s] is not a file"%(self.ifpath))
        # 输出路径已存在
        if os.path.exists(self.ofpath):
            self.__die("output [%s] is already exists"%(self.ofpath))
        if self.factor < 1 or self.factor > 10:
            self.__die("factor [%d] is out of range [2, 10]"%(self.factor))
        
    def doParse(self):
        self.config = ConfigObj(self.ifpath, encoding='UTF8')
        if self.config == None:
            self.__die("input [%s] is not a valid ini file"%(self.ifpath))
        
    def doExpand(self):
        # 每一条 acl
        for section in self.config:  
            if self.config[section].get('pkb_burst', default=None):
                self.config[section]['pkb_burst'] = self.factor*int(self.config[section]['pkb_burst'])
            if self.config[section].get('pkb_sec', default=None):
                self.config[section]['pkb_sec'] = self.factor*int(self.config[section]['pkb_sec'])
            if self.config[section].get('ckb_burst', default=None):
                self.config[section]['ckb_burst'] = self.factor*int(self.config[section]['ckb_burst'])
            if self.config[section].get('ckb_sec', default=None):
                self.config[section]['ckb_sec'] = self.factor*int(self.config[section]['ckb_sec'])
        # 保存文件
        __filename = self.config.filename
        self.config.filename = self.ofpath
        self.config.write()
        self.config.filename = __filename

    def __die(self, message):
        print(message)
        exit(-1)

    @staticmethod
    def help(name):
        print("usage: %s <input ini> <output ini> <factor from 2 to 10>"%(name))

if __name__ == '__main__':
    # 1. 参数简单, 不提供选项, 仅支持固定顺序传入
    if len(sys.argv) != 4:
        ExpandIfpcos.help(sys.argv[0])
    mExpandIfpcos = ExpandIfpcos(sys.argv[1], sys.argv[2], int(sys.argv[3]))
    mExpandIfpcos.doCheck()
    mExpandIfpcos.doParse()
    mExpandIfpcos.doExpand()
    pass