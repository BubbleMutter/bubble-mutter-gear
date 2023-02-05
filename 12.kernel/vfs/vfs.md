# vfs 文件抽象概念 (从 unix 继承)
1. 文件 = byte-stream 的集合; 支持 读/写/创建/删除 等操作
2. 目录项 = 包含文件和其他目录 的集合; 目录层层嵌套形成文件路经
3. 索引节点 = 文件 metatdata 与文件本身数据抽离; 包含 权限控制/大小/修改时间 等
4. 挂载点 = 文件系统必须挂载在全局层次结构的一个节点中; 即枝叶

# vfs 关键对象类型
1. super_block = 表示一个 mount 的 filesystem
   1. 记录 filesystem 的 metadata (文件系统类型/大小/状态 等)
   2. 原始信息保存在 disk 中; kernel 运行时序列化到内存
2. inode = 表示一个 文件 or 目录节点
   1. 记录 file 的 metadata (owner group type 但不包含文件名)
   2. 原始信息保存在 disk 中; kernel 运行时序列化到内存
3. dentry = 表示一个目录; 路经的组成部分
   1. 只有在 kernel 运行时的内存中
4. file = 进程打开的文件对象
   1. 只有在 kernel 运行时的内存中

# vfs 关键接口定义
1. super_operations = 内核针对文件系统的方法
2. inode_operations = 内核针对文件节点的方法
3. dentry_operations = 内核针对目录的方法
4. file_operations = 进程针对已打开文件的调用方法

# vfs 其他对象类型
## struct vfsmount
```c++
struct vfsmount {
	struct dentry *mnt_root;	/* root of the mounted tree */
	struct super_block *mnt_sb;	/* pointer to superblock */
	int mnt_flags;
} __randomize_layout;
```
+ `vfsmount->mnt_sb` 挂载节点的文件系统

# vfs 目录项设计
1. 路径 `/bin/vi` 中, "/", "bin", "vi" 三者都是"文件"
   1. 每个 "文件" 都有对应的 inode
   2. 每个 inode 对应一个 d_entry