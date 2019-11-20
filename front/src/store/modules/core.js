import api from '../../api'

const state = {
  power: null,
}

const getters = {}

const actions = {
  get_power({commit}) {
    api.power.get()
      .then(response => {
        commit("update_power", response.data.power === 1);
      })
      .catch(error => {
        console.log(error);
      });
  },
  set_power({commit}, newPowerValue) {
    api.power.set({power: newPowerValue === true ? 1 : 0})
      .then(response => {
        commit("update_power", newPowerValue);
        console.log(response);
      })
      .catch(error => {
        console.log(error);
      });
  }
}

const mutations = {
  update_power(state, newValue) {
    state.power = newValue;
  }
}

export default {
  namespaced: true,
  state,
  getters,
  actions,
  mutations
}
