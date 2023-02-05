npm init -y
touch index.js
npm install --save-dev express
npm install --save-dev babel-cli babel-preset-es2015 rimraf
npm install --save-dev babel-preset-stage-0 # await/async: babel-plugin-syntax-async-functions、babel-plugin-transform-regenerator
npm install --save-dev babel-polyfill babel-plugin-transform-runtime babel-runtime
npm install --save-dev eslint eslint-config-airbnb

# yarn会多一级本地缓存, 而且命令少敲一点

