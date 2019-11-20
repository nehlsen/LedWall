import api from '../../api'

const state = {
  modes: [],
  mode: null,
}

const getters = {}

const actions = {
  all({commit}) {
    api.mode.all()
      .then(response => {
        commit("update_modes", response.data.modes);
      })
      .catch(error => {
        console.log(error);
      });
  },
  get({commit}) {
    api.mode.get()
      .then(response => {
        commit("update_mode", response.data.index);
      })
      .catch(error => {
        console.log(error);
      });
  },
  set({commit}, newModeIndex) {
    api.mode.set({index: newModeIndex})
      .then(response => {
        commit("update_mode", newModeIndex);
        console.log(response);
      })
      .catch(error => {
        console.log(error);
      });
  }
}

const mutations = {
  update_modes(state, newValue) {
    state.modes = newValue;
  },
  update_mode(state, newValue) {
    state.mode = newValue;
  }
}

export default {
  namespaced: true,
  state,
  getters,
  actions,
  mutations
}
