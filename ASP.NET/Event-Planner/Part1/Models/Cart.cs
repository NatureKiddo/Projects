using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace Event_Manager.Models
{
    public class Cart
    {
        // Properties for events in the cart
        public int EventId { get; set; } // Unique identifier for the event in the cart
        public string EventName { get; set; } // Name of the event in the cart
        public int Quantity { get; set; } // Quantity of the event in the cart
        public decimal TotalPrice { get; set; } // Total price of the event(s) in the cart
    }
}
