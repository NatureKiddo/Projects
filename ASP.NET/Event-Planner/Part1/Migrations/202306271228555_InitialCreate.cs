using System;
using System.Collections.Generic;
using System.Data.Common;
using System.Data.Entity.Migrations;
using System.Linq;
using System.Web;

namespace Event_Manager.Migrations
{
    public partial class InitialCreate : DbMigration
    {
        public override void Up()
        {
            // Create the Events table
            CreateTable(
                "dbo.Events",
                c => new
                {
                    Id = c.Int(nullable: false, identity: true),
                    EventName = c.String(nullable: false),
                    Location = c.String(),
                    Price = c.Decimal(nullable: false, precision: 18, scale: 2),
                    CodeValue = c.String(nullable: false, maxLength: 50),
                    MaxUsage = c.Int(),
                    DiscountAmount = c.Decimal(nullable: false, precision: 18, scale: 2)
                })
                .PrimaryKey(t => t.Id);
        }

        public override void Down()
        {
            DropTable("dbo.Events");
        }
    }
}