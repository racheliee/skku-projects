"use strict";

// This will be the object that will contain the Vue attributes
// and be used to initialize it.
let app = {};

app.data = {
  data: function () {
    return {
      contacts: [],
    };
  },
  methods: {
    // Complete.
    load_contacts: function () {
      fetch(get_contacts_url)
        .then((res) => res.json())
        .then((data) => {
          this.contacts = data.contacts.map((contact) => ({
            ...contact,
            editingName: false,
            editingAffiliation: false,
            editingDescription: false,
          }));
        });
    },
    addContact() {
        fetch(add_contact_url, { method: "POST" })
            .then(res => res.json())
            .then(data => {
                this.contacts.push(data.contact);
            });
    },
    deleteContact(contact) {
        fetch(delete_contact_url, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ id: contact.id })
        })
        .then(response => {
            if (response.ok) {
                this.contacts = this.contacts.filter(c => c.id !== contact.id);
            } else {
                console.error("Failed to delete contact");
            }
        })
        .catch(error => console.error("Error:", error));
    },
    editField(contact, field) {
        contact[`editing${field.charAt(0).toUpperCase() + field.slice(1)}`] = true;
    },
    saveContact(contact, field) {
        contact[`editing${field.charAt(0).toUpperCase() + field.slice(1)}`] = false;
        fetch(update_contact_url, {
            method: "POST",
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(contact)
        });
    },
    chooseImage(contact) {
        const input = document.getElementById("file-input");
        input.onchange = () => {
            const file = input.files[0];
            if (file) {
                const reader = new FileReader();
                reader.onload = () => {
                    contact.photo_url = reader.result;
                    fetch(update_contact_url, {
                        method: "POST",
                        headers: { 'Content-Type': 'application/json' },
                        body: JSON.stringify({ id: contact.id, photo_url: reader.result })
                    });
                };
                reader.readAsDataURL(file);
            }
        };
        input.click();
    }
  },
  mounted() {
    this.load_contacts();
  }
};

app.vue = Vue.createApp(app.data).mount("#app");

app.load_data = function () {
  // Complete.
};

app.load_data();
