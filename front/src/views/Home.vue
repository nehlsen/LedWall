<template>
  <v-container>
    <v-layout text-xs-center wrap>
      <v-flex xs12 sm6 offset-sm3>
        <v-card>
          <v-img :src="require('../assets/logo.png')" contain height="200"></v-img>
          <v-card-title primary-title>
            <div class="ma-auto">
              <span class="grey--text">IDF version: {{idfVersion}}</span>
              <br>
              <span class="grey--text">ESP cores: {{cores}}</span>
              <br>
              <span class="grey--text">System restarts: {{restarts}}</span>
              <br>
              <span class="grey--text">Project Name: {{projectName}}</span>
              <br>
              <span class="grey--text">Project Version: {{projectVersion}}</span>
              <br>
              <span class="grey--text">Compiled: {{compileTime}} / {{compileDate}}</span>
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
      sysInfo: null,
      idfVersion: null,
      cores: null,
      restarts: null,
      projectName: null,
      projectVersion: null,
      compileDate: null,
      compileTime: null
    }
  },
  mounted () {
    this.$ajax
      .get('/api/v1/system/info')
      .then(data => {
        this.sysInfo = data.data;
        this.idfVersion = data.data.idfVersion;
        this.cores = data.data.cores;
        this.restarts = data.data.restarts;
        this.projectName = data.data.projectName;
        this.projectVersion = data.data.projectVersion;
        this.compileTime = data.data.compileTime;
        this.compileDate = data.data.compileDate;
      })
      .catch(error => {
        console.log(error);
      });
  }
};
</script>
