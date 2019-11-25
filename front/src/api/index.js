import axios from 'axios'

export const power = {
  get: () => axios.get('/api/v1/led/power'),
  set: (data) => axios.post('/api/v1/led/power', data),
};

export const mode = {
  all: () => axios.get('/api/v1/led/modes'),
  get: () => axios.get('/api/v1/led/mode'),
  set: (data) => axios.post('/api/v1/led/mode', data),
};

export const config = {
  get: () => axios.get('/api/v1/config'),
  set: (data) => axios.post('/api/v1/config', data),
};

export default {
  power,
  mode,
  config,
}
