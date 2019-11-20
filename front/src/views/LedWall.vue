<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card>
          <v-card-title primary-title>
            <div class="ma-auto">
              <v-switch v-model="power" label="Power"></v-switch>
              <br>
              <v-select :items="modes" v-model="mode" label="Mode" item-text="name" item-value="index"></v-select>
            </div>
          </v-card-title>
        </v-card>
      </v-flex>
    </v-layout>
  </v-container>
</template>

<script>
export default {
  data () {
    return {
      modes: [],
      mode: null
    }
  },
  computed: {
    power: {
      get: function() {
        return this.$store.state.power;
      },
      set: function(newValue) {
        this.$store.dispatch("set_power", newValue);
      }
    }
  },
  created() {
    this.$store.dispatch("get_power");
  },
  mounted () {
    this.$ajax
      .get('/api/v1/led/modes')
      .then(data => {
        this.modes = data.data.modes;
      })
      .catch(error => {
        console.log(error);
      });
    this.$ajax
      .get('/api/v1/led/mode')
      .then(data => {
        this.mode = data.data.index;
      })
      .catch(error => {
        console.log(error);
      });
  },
  watch: {
    mode() {
      this.set_mode();
    }
  },
  methods: {
    set_mode: function() {
      this.$ajax
        .post('/api/v1/led/mode', {
          index: this.mode
        })
        .then(data => {
          console.log(data);
        })
        .catch(error => {
          console.log(error);
        });
    }
  }
};
/*
{
"modes": [
{
  "index": 0,
  "name": "Status"
},
{
  "index": 1,
  "name": "Sample"
},
{
  "index": 2,
  "name": "Hsiboy"
}
]
}
 */

</script>

