import axios from 'axios'

export const power = {
  get: () => axios.get('/api/v1/led/power'),
  set: (data) => axios.post('/api/v1/led/power', data),
};

export default {
  power: power
}
