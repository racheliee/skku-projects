"use strict";

// This will be the object that will contain the Vue attributes
// and be used to initialize it.
let app = {};

// Vue data and methods
app.data = {
  data: function () {
    return {
      posts: [],
      tags: [],
      activeTags: [],
      postContent: "",
    };
  },
  computed: {
    filteredPosts: function () {
      if (this.activeTags.length === 0) {
        return this.posts;
      }
      return this.posts.filter((post) =>
        post.tags.some((tag) => this.activeTags.includes(tag))
      );
    },
  },
  methods: {
    fetchPosts: function () {
      fetch(get_posts_url)
        .then((res) => res.json())
        .then((data) => {
          this.posts = data.posts.map((post) => ({
            ...post,
            tags: post.tags ? post.tags.split(",") : [],
          }));
        });
    },
    createPost() {
      fetch(create_post_url, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ content: this.postContent }),
      })
        .then((res) => res.json())
        .then((data) => {
          if (data.error) {
            console.error(data.error);
          } else {
            this.posts.unshift({
              id: data.post_id,
              content: this.postContent,
              tags: data.tags || [],
            });
            this.postContent = ""; // Clear the input
          }
        })
        .catch((error) => console.error("Error creating post:", error));
    },
    deletePost: async function (postId) {
      fetch(`${delete_post_url}/${postId}`, {
        method: "DELETE",
      })
        .then((response) => {
          if (response.ok) {
            this.posts = this.posts.filter((post) => post.id !== postId);
          } else {
            console.error("Failed to delete post");
          }
        })
        .catch((error) => console.error("Error:", error));
    },
    fetchTags: async function () {
      fetch(get_tags_url)
        .then((res) => res.json())
        .then((data) => {
          this.tags = data.tags;
        });
    },
    toggleTag: function (tag) {
      const index = this.activeTags.indexOf(tag);
      if (index === -1) {
        this.activeTags.push(tag);
      } else {
        this.activeTags.splice(index, 1);
      }
    },
  },
  mounted: async function () {
    await this.fetchPosts();
    await this.fetchTags();
  },
};

// Vue app initialization
app.vue = Vue.createApp(app.data).mount("#app");

// Initialize the app
app.load_data = function () {
  console.log("App loaded and initialized."); // Debug log
};

app.load_data();
