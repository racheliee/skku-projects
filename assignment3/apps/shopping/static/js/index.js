"use strict";

// This will be the object that will contain the Vue attributes
// and be used to initialize it.
let app = {};


app.data = {
    data: function() {
        return {
            // Complete as you see fit.
        };
    },
    methods: {
        // Complete as you see fit.
        },
};

app.vue = Vue.createApp(app.data).mount("#app");

app.load_data = function () {
    // Complete.
}

// This is the initial data load.
app.load_data();

