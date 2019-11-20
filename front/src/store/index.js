import Vue from 'vue'
import Vuex from 'vuex'
import core from './modules/core'

Vue.use(Vuex)

const debug = process.env.NODE_ENV !== 'production'

export default new Vuex.Store({
  modules: {
    core
  },
  strict: debug
})
