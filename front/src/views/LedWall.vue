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
  computed: {
    power: {
      get: function() {
        return this.$store.state.core.power;
      },
      set: function(newValue) {
        this.$store.dispatch("core/set_power", newValue);
      }
    },
    mode: {
      get: function() {
        return this.$store.state.mode.mode;
      },
      set: function(newValue) {
        this.$store.dispatch("mode/set", newValue);
      }
    },
    modes: function() {
      return this.$store.state.mode.modes;
    }
  },
  created() {
    this.$store.dispatch("core/get_power");
    this.$store.dispatch("mode/all");
    this.$store.dispatch("mode/get");
  }
};
</script>

