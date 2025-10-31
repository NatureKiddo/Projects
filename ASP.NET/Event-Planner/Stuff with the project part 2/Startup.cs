using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(Event_Manager.Startup))]
namespace Event_Manager
{
    public partial class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            ConfigureAuth(app);
        }
    }
}
