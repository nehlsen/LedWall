<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card>
          <v-card-title primary-title>
            <div class="ma-auto">
              <v-select :items="powerOnResetModes" v-model="powerOnResetMode" label="Power-On Reset Mode" item-text="title" item-value="value"></v-select>
              <br>
              <v-switch v-model="ledModeAutoRestore" label="Restore Last LED Mode"></v-switch>
              <v-select :disabled="this.ledModeAutoRestore === true" :items="ledModes" v-model="ledModeBootInto" label="Boot into LED Mode" item-text="name" item-value="index"></v-select>
            </div>
          </v-card-title>
        </v-card>
      </v-flex>
    </v-layout>
  </v-container>
</template>

<script>
export default {
  data() {
    return {
      ledModes: [],
      ledModeBootInto: null,
      powerOnResetModes: [
        {"title": "Always off", "value": 0},
        {"title": "Always on", "value": 1},
        {"title": "Recover last State", "value": 2},
      ]
    };
  },
  computed: {
    powerOnResetMode: {
      get: function() {
        return this.$store.state.config.PowerOnResetMode;
      },
      set: function(newValue) {
        this.$store.dispatch("config/set", {"PowerOnResetMode": newValue});
      }
    },
    ledModeAutoRestore: {
      get: function() {
        return this.$store.state.config.LedModeAutoRestore === -1;
      },
      set: function(newValue) {
        this.$store.dispatch("config/set", {"LedModeAutoRestore": newValue ? -1 : 0});
      }
    }
  },
  created() {
    this.$store.dispatch("config/get");
    this.ledModes = this.$store.state.mode.modes;
  }
};
</script>

