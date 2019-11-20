import Vue from 'vue'
import Vuex from 'vuex'
import axios from 'axios'

Vue.use(Vuex)

export default new Vuex.Store({
  state: {
    power: null,
  },
  mutations: {
    update_power(state, newValue) {
      state.power = newValue;
    }
  },
  actions: {
    get_power({commit}) {
      axios
        .get('/api/v1/led/power')
        .then(data => {
          commit("update_power", data.data.power);
        })
        .catch(error => {
          console.log(error);
        });
    },
    set_power({commit}, newPowerValue) {
      axios
        .post('/api/v1/led/power', {
          power: newPowerValue === true ? 1 : 0
        })
        .then(data => {
          commit("update_power", newPowerValue);
          console.log(data);
        })
        .catch(error => {
          console.log(error);
        });
    }
  }
})
