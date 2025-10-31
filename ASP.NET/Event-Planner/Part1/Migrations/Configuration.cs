using System;
using System.Collections.Generic;
using System.Data.Entity.Migrations;
using System.Linq;
using System.Web;

namespace Event_Manager.Migrations
{
    internal sealed class Configuration : DbMigrationsConfiguration<Event_Manager.Data.EventManager_Data>
    {
        public Configuration()
        {
            AutomaticMigrationsEnabled = true;
            ContextKey = "Event_Manager.Data.EventManager_Data";
        }

        protected override void Seed(Event_Manager.Data.EventManager_Data context)
        {
            //  This method will be called after migrating to the latest version.

            //  You can use the DbSet<T>.AddOrUpdate() helper extension method
            //  to avoid creating duplicate seed data.
        }
    }
}