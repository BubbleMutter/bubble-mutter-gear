$(function() { // 页面加载完成时的回调
    // get 请求
    $.get(uri, function(message, retcode) {
        console.log(message); // 返回的数据
        console.log(retcode); // http状态码
    });
    // post 请求
    $.post(uri, function(message, retcode) {
        console.log(message); // 返回的数据
        console.log(retcode); // http状态码
    });

    // 通过 元素 id 获取; 对应一个 button 的点击事件
    $("#send").on("click", function() {
        console.log("`send` has been clicked"); 
    });
});