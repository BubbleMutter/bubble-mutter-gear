<?php

// 获取请求方法 ----------------------------------------
switch ($_SERVER['REQUEST_METHOD']) {
    case 'POST':
    case 'GET':
    default:
}
// --------------------------------------------------

// 获取 Post 字段 -------------------------------------
$uuid = $_POST['uuid'];
$data = $_POST['data'];
// --------------------------------------------------

// 获取请求参数
parse_str($_SERVER["QUERY_STRING"], $QUERY_TABLE);
$search = $QUERY_TABLE['search'];
// --------------------------------------------------

// 返回 header ----------------------------------------
// 原生的方法 并不好, 容易出错, 建议用框架的
header('Content-Type: text/plain; charset=utf-8'); //网页编码
// --------------------------------------------------

// 返回 json; 实际上是 dict 转 json 字符串 --------------
echo json_encode(array(
    'retcode' => 0,
    'message' => 'success',
    'data' => 'introduction to php'
));
// --------------------------------------------------