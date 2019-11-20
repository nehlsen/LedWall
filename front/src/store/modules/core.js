import api from '../../api'

const state = {
  power: null,
}

const getters = {}

const actions = {
  get_power({commit}) {
    api.power.get()
      .then(data => {
        commit("update_power", data.data.power);
      })
      .catch(error => {
        console.log(error);
      });
  },
  set_power({commit}, newPowerValue) {
    api.power.set({power: newPowerValue === true ? 1 : 0})
      .then(data => {
        commit("update_power", newPowerValue);
        console.log(data);
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
