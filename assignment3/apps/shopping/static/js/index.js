"use strict";

// This will be the object that will contain the Vue attributes
// and be used to initialize it.
let app = {};

app.data = {
  data: function () {
    return {
      newItem: "",
      items: [], // stores the items in the shopping list
    };
  },
  computed: {
    sortedItems: function () {
      // return unchecked items first, then sort by id
      return this.items.sort((a, b) => {
        if (a.purchased !== b.purchased) {
          return a.purchased - b.purchased; // Unchecked items (false) before checked items (true)
        }
        if (!a.purchased) {
          // For unchecked items, sort by last_modified in descending order (newest first)
          return new Date(b.last_modified) - new Date(a.last_modified);
        } else {
          // For checked items, sort by last_modified in ascending order (oldest first)
          return a.id - b.id;
        }
      });
    },
  },
  methods: {
    loadItems: function () {
      fetch(load_data_url)
        .then((response) => response.json())
        .then((data) => {
          this.items = data.items;
        });
    },
    addItem: function () {
      if (this.newItem.trim()) {
        fetch(add_item_url, {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
          },
          body: JSON.stringify({
            item_name: this.newItem.trim(),
          }),
        }).then(() => {
          this.newItem = "";
          this.loadItems();
        });
      }
    },
    toggleItem: function (item) {
      fetch(toggle_item_url, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ item_id: item.id }),
      }).then(() => {
        this.loadItems();
      });
    },
    deleteItem: function (item) {
      fetch(delete_item_url, {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ item_id: item.id }),
      }).then(() => {
        this.loadItems();
      });
    },
  },
};

app.vue = Vue.createApp(app.data).mount("#app");

app.load_data = function () {
  // Complete.
  app.vue.loadItems();
};

// This is the initial data load.
app.load_data();
