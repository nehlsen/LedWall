import api from '../../api'

const state = {
  PowerOnResetMode: null,
  LedModeAutoRestore: null, // one of modes or -1 for restore last
}

const getters = {}

const actions = {
  get({commit}) {
    api.config.get()
      .then(response => {
        commit("update_config", response.data);
      })
      .catch(error => {
        console.log(error);
      });
  },
  set({commit}, newConfig) {
    api.config.set(newConfig)
      .then(response => {
        commit("update_config", newConfig);
        console.log(response);
      })
      .catch(error => {
        console.log(error);
      });
  }
}

const mutations = {
  update_config(state, newConfig) {
    state.PowerOnResetMode = newConfig.PowerOnResetMode;
    state.LedModeAutoRestore = newConfig.LedModeAutoRestore;
  }
}

export default {
  namespaced: true,
  state,
  getters,
  actions,
  mutations
}
