using System;
using System.Collections.Generic;
using System.Data;
using System.Data.Entity;
using System.Linq;
using System.Net;
using System.Web;
using System.Web.Mvc;
using Event_Manager.Data;
using Event_Manager.Models;

namespace Event_Manager.Controllers
{
    public class EventsController : Controller
    {
        private EventManager_Data db = new EventManager_Data();

        // GET: Events
        public ActionResult Events_Lists()
        {
            return View(db.Events.ToList());
        }

        // GET: Events/Details/5
        public ActionResult Details(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            Event @event = db.Events.Find(id);
            if (@event == null)
            {
                return HttpNotFound();
            }
            return View(@event);
        }

        // GET: Events/Create
        public ActionResult Create()
        {
            return View();
        }

        // POST: Events/Create
        // To protect from overposting attacks, enable the specific properties you want to bind to, for 
        // more details see https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Create([Bind(Include = "Id,EventName,Location,Price")] Event @event)
        {
            if (ModelState.IsValid)
            {
                db.Events.Add(@event);
                db.SaveChanges();
                return RedirectToAction("Index");
            }

            return View(@event);
        }

        // GET: Events/Edit/5
        public ActionResult Edit(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            Event @event = db.Events.Find(id);
            if (@event == null)
            {
                return HttpNotFound();
            }
            return View(@event);
        }

        // POST: Events/Edit/5
        // To protect from overposting attacks, enable the specific properties you want to bind to, for 
        // more details see https://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public ActionResult Edit([Bind(Include = "Id,EventName,Location,Price")] Event @event)
        {
            if (ModelState.IsValid)
            {
                db.Entry(@event).State = EntityState.Modified;
                db.SaveChanges();
                return RedirectToAction("Index");
            }
            return View(@event);
        }

        // GET: Events/Delete/5
        public ActionResult Delete(int? id)
        {
            if (id == null)
            {
                return new HttpStatusCodeResult(HttpStatusCode.BadRequest);
            }
            Event @event = db.Events.Find(id);
            if (@event == null)
            {
                return HttpNotFound();
            }
            return View(@event);
        }

        // POST: Events/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public ActionResult DeleteConfirmed(int id)
        {
            Event @event = db.Events.Find(id);
            db.Events.Remove(@event);
            db.SaveChanges();
            return RedirectToAction("Index");
        }



        // Add an event to the cart
        public ActionResult AddToCart(int id)
        {
            Event @event = db.Events.Find(id); // Use the Event model

            if (@event == null)
            {
                return HttpNotFound();
            }

            Cart cartItem = new Cart
            {
                EventId = @event.Id,
                EventName = @event.EventName, // Use EventName property from the Event model
                Quantity = 1,
                TotalPrice = @event.Price // Use Price property from the Event model
            };

            List<Cart> cart = Session["Cart"] as List<Cart>;

            if (cart == null)
            {
                cart = new List<Cart>();
            }

            cart.Add(cartItem);

            decimal totalPrice = cart.Sum(c => c.TotalPrice);

            foreach (var item in cart)
            {
                item.TotalPrice = item.Quantity * item.TotalPrice;
            }

            Session["TotalPrice"] = totalPrice;
            Session["Cart"] = cart;

            return RedirectToAction("ViewCart");
        }

        // View the cart and its contents
        public ActionResult ViewCart()
        {
            List<Cart> cart = Session["Cart"] as List<Cart>;

            if (cart == null || cart.Count == 0)
            {
                ViewBag.Message = "Your cart is empty.";
            }
            else
            {
                decimal totalPrice = cart.Sum(c => c.TotalPrice);
                ViewBag.TotalPrice = totalPrice;
            }

            return View(cart);
        }

        // Remove an event from the cart
        public ActionResult RemoveFromCart(int id)
        {
            List<Cart> cart = Session["Cart"] as List<Cart>;

            if (cart != null)
            {
                var cartItem = cart.FirstOrDefault(c => c.EventId == id);

                if (cartItem != null)
                {
                    cart.Remove(cartItem);
                    Session["Cart"] = cart;
                }
            }

            return RedirectToAction("ViewCart");
        }

        // Apply promo code
        public ActionResult ApplyPromoCode(string promoCode)
        {
            Event discountEvent = db.Events.FirstOrDefault(e => e.CodeValue == promoCode);

            if (discountEvent != null)
            {
                decimal discountAmount = discountEvent.DiscountAmount;
                decimal totalPrice = (decimal)Session["TotalPrice"];
                decimal discountedPrice = totalPrice - discountAmount;

                if (discountedPrice < 0)
                {
                    discountedPrice = 0;
                }

                // Apply the discount to each item in the cart
                List<Cart> cart = Session["Cart"] as List<Cart>;
                if (cart != null)
                {
                    foreach (var item in cart)
                    {
                        item.TotalPrice = item.Quantity * item.TotalPrice;
                    }

                    ViewBag.TotalPrice = discountedPrice; // Update ViewBag.TotalPrice
                    Session["TotalPrice"] = discountedPrice;

                    ViewBag.Message = $"Promo code '{promoCode}' applied. You received a {discountAmount:C} discount!";
                }
                else
                {
                    ViewBag.Message = "Your cart is empty.";
                }
            }
            else
            {
                ViewBag.Message = "Invalid promo code. Please try again.";
            }

            // Immediately update the view with the changes
            return View("ViewCart", Session["Cart"] as List<Cart>);
        }


        // Dispose of the database context
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                db.Dispose();
            }
            base.Dispose(disposing);
        }
    }
}