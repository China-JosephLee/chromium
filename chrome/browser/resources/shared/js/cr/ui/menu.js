// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

cr.define('cr.ui', function() {

  /** @const */ var MenuItem = cr.ui.MenuItem;

  /**
   * Creates a new menu element. Menu dispatches all commands on the element it
   * was shown for.
   *
   * @param {Object=} opt_propertyBag Optional properties.
   * @constructor
   * @extends {HTMLMenuElement}
   */
  var Menu = cr.ui.define('menu');

  Menu.prototype = {
    __proto__: HTMLMenuElement.prototype,

    selectedIndex_: -1,

    /**
     * Element for which menu is being shown.
     */
    contextElement: null,

    /**
     * Initializes the menu element.
     */
    decorate: function() {
      this.addEventListener('mouseover', this.handleMouseOver_);
      this.addEventListener('mouseout', this.handleMouseOut_);

      this.classList.add('decorated');
      this.setAttribute('role', 'menu');
      this.hidden = true;  // Hide the menu by default.

      // Decorate the children as menu items.
      var children = this.children;
      for (var i = 0, child; child = children[i]; i++) {
        cr.ui.decorate(child, MenuItem);
      }
    },

    /**
     * Adds menu item at the end of the list.
     * @param {Object} item Menu item properties.
     * @return {cr.ui.MenuItem} The created menu item.
     */
    addMenuItem: function(item) {
      var menuItem = this.ownerDocument.createElement('menuitem');
      this.appendChild(menuItem);

      cr.ui.decorate(menuItem, MenuItem);

      if (item.label)
        menuItem.label = item.label;

      if (item.iconUrl)
        menuItem.iconUrl = item.iconUrl;

      return menuItem;
    },

    /**
     * Adds separator at the end of the list.
     */
    addSeparator: function() {
      var separator = this.ownerDocument.createElement('hr');
      this.appendChild(separator);
    },

    /**
     * Clears menu.
     */
    clear: function() {
      this.textContent = '';
    },

    /**
     * Walks up the ancestors of |el| until a menu item belonging to this menu
     * is found.
     * @param {Element} el The element to start searching from.
     * @return {cr.ui.MenuItem} The found menu item or null.
     * @private
     */
    findMenuItem_: function(el) {
      while (el && el.parentNode != this) {
        el = el.parentNode;
      }
      return el;
    },

    /**
     * Handles mouseover events and selects the hovered item.
     * @param {Event} e The mouseover event.
     * @private
     */
    handleMouseOver_: function(e) {
      var overItem = this.findMenuItem_(e.target);
      this.selectedItem = overItem;
    },

    /**
     * Handles mouseout events and deselects any selected item.
     * @param {Event} e The mouseout event.
     * @private
     */
    handleMouseOut_: function(e) {
      this.selectedItem = null;
    },

    /**
     * The selected menu item or null if none.
     * @type {cr.ui.MenuItem}
     */
    get selectedItem() {
      return this.children[this.selectedIndex];
    },
    set selectedItem(item) {
      var index = Array.prototype.indexOf.call(this.children, item);
      this.selectedIndex = index;
    },

    /**
     * Focuses the selected item. If selectedIndex is invalid, set it to 0
     * first.
     */
    focusSelectedItem: function() {
      if (this.selectedIndex < 0 ||
          this.selectedIndex > this.children.length) {
        this.selectedIndex = 0;
      }

      if (this.selectedItem)
        this.selectedItem.focus();
    },

    /**
     * Menu length
     */
    get length() {
      return this.children.length;
    },

    /**
     * This is the function that handles keyboard navigation. This is usually
     * called by the element responsible for managing the menu.
     * @param {Event} e The keydown event object.
     * @return {boolean} Whether the event was handled be the menu.
     */
    handleKeyDown: function(e) {
      var item = this.selectedItem;

      var self = this;
      function selectNextAvailable(m) {
        var children = self.children;
        var len = children.length;
        var i = self.selectedIndex;
        if (i == -1 && m == -1) {
          // Edge case when needed to go the last item first.
          i = 0;
        }

        // "i" may be negative(-1), so modulus operation and cycle below
        // wouldn't work as assumed. This trick makes startPosition positive
        // without altering it's modulo.
        var startPosition = (i + len) % len;

        while (true) {
          i = (i + m + len) % len;

          // Check not to enter into infinite loop if all items are hidden or
          // disabled.
          if (i == startPosition)
            break;

          item = children[i];
          if (item && !item.isSeparator() && !item.hidden && !item.disabled)
            break;
        }
        if (item && !item.disabled)
          self.selectedIndex = i;
      }

      switch (e.keyIdentifier) {
        case 'Down':
          selectNextAvailable(1);
          return true;
        case 'Up':
          selectNextAvailable(-1);
          return true;
        case 'Enter':
        case 'U+0020': // Space
          if (item) {
            var activationEvent = cr.doc.createEvent('Event');
            activationEvent.initEvent('activate', true, true);
            activationEvent.originalEvent = e;
            if (item.dispatchEvent(activationEvent)) {
              if (item.command)
                item.command.execute();
            }
          }
          return true;
      }

      return false;
    },

    /**
     * Updates menu items command according to context.
     * @param {Node=} node Node for which to actuate commands state.
     */
    updateCommands: function(node) {
      var children = this.children;

      for (var i = 0, child; child = children[i]; i++)
        child.updateCommand(node);
    }
  };

  function selectedIndexChanged(selectedIndex, oldSelectedIndex) {
    var oldSelectedItem = this.children[oldSelectedIndex];
    if (oldSelectedItem)
      oldSelectedItem.selected = false;
    var item = this.selectedItem;
    if (item) {
      item.selected = true;
      this.focusSelectedItem();
    }
  }

  /**
   * The selected menu item.
   * @type {number}
   */
  cr.defineProperty(Menu, 'selectedIndex', cr.PropertyKind.JS,
      selectedIndexChanged);

  // Export
  return {
    Menu: Menu
  };
});
