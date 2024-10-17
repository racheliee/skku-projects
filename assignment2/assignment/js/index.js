"use strict";

let app = {};

app.data = {    
    data: function() {
        return {
            x: 2 // Example
        };
    },
    methods: {    
        something: function() {
            console.log("Hello World");
        },
    }
};

app.vue = Vue.createApp(app.data).mount("#app");
app.vue.recompute();

