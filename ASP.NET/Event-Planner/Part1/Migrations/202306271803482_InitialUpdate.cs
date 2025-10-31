using System;
using System.Collections.Generic;
using System.Data.Entity.Migrations;
using System.Linq;
using System.Web;

namespace Event_Manager.Migrations
{
    public partial class InitialUpdate : DbMigration
    {
        public override void Up()
        {
            AlterColumn("dbo.Events", "Price", c => c.String(nullable: false));
        }

        public override void Down()
        {
            AlterColumn("dbo.Events", "Price", c => c.String());
        }
    }
}
