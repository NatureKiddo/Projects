using Event_Manager.Models;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace Event_Manager.Data
{
    public class EventManager_Data : DbContext
    {
        // GET: EventManager_Data
        public EventManager_Data() : base("name=EventManager_Data")
        {
        }

        public System.Data.Entity.DbSet<Event_Manager.Models.Event> Events { get; set; }


    }
}