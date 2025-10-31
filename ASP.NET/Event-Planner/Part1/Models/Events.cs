using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace Event_Manager.Models
{
    public class Event
    {
        [Key]
        public int Id { get; set; } // Primary key for the event

        [Required(ErrorMessage = "Event name is required.")]
        public string EventName { get; set; } // Name of the event, required field

        public string Location { get; set; } // Location of the event

        [Required(ErrorMessage = "Price is required.")]
        public decimal Price { get; set; } // Price per ticket, required field

        public string CodeValue { get; set; } // CodeValue from the SQL table
        public int MaxUsage { get; set; } // MaxUsage from the SQL table
        public decimal DiscountAmount { get; set; } // DiscountAmount from the SQL table
    }
}
