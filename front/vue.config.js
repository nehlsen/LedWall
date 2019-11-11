module.exports = {
  productionSourceMap: false,
  devServer: {
    proxy: {
      '/api': {
        target: 'http://10.13.37.222:80',
        changeOrigin: true,
        ws: true
      }
    }
  }
}
