import Vue from 'vue'
import Router from 'vue-router'
import Home from './views/Home'
import Chart from './views/Chart'
import Light from './views/Light'
import LedWall from './views/LedWall'
import Config from './views/Config'

Vue.use(Router)

export default new Router({
  mode: 'history',
  base: process.env.BASE_URL,
  routes: [
    {
      path: '/',
      name: 'home',
      component: Home
    },
    {
      path: '/ledwall',
      name: 'ledwall',
      component: LedWall
    },
    {
      path: '/chart',
      name: 'chart',
      component: Chart
    },
    {
      path: '/light',
      name: 'light',
      component: Light
    },
    {
      path: '/config',
      name: 'config',
      component: Config
    }
  ]
})
