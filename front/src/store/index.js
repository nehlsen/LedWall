import Vue from 'vue'
import Vuex from 'vuex'
import core from './modules/core'
import mode from './modules/mode'

Vue.use(Vuex)

const debug = process.env.NODE_ENV !== 'production'

export default new Vuex.Store({
  modules: {
    core,
    mode
  },
  strict: debug
})
