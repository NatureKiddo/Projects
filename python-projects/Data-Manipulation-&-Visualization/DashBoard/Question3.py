import numpy as np  
import pandas as pd
import matplotlib.pyplot as plt   
import seaborn as sns   

#Loading the Superstore dataset
file_path = r'C:\Users\Revy\Desktop\RichField\Data Manipulation & Visulation\600\Assisgnment\Sample - Superstore.csv'
df = pd.read_csv(file_path, encoding='latin1')

# Question 3.1.

#Calculting Sales and Profit using mean, median and standard deviation
sales_array = df['Sales'].to_numpy()
profit_array = df['Profit'].to_numpy()

#Calculating the Sales
sales_mean = np.mean(sales_array)
sales_median = np.median(sales_array)
sales_std = np.std(sales_array)

#Calculating the Profit
profit_mean = np.mean(profit_array)
profit_median = np.median(profit_array)
profit_std = np.std(profit_array)

print(f"Sales - Mean: {sales_mean:.2f}, Median: {sales_median:.2f}, Standard Deviation: {sales_std:.2f}")
print(f"Profit - Mean: {profit_mean:.2f}, Median: {profit_median:.2f}, Standard Deviation: {profit_std:.2f}")

#25th, 50th and 75th percentiles
sales_percentiles = np.percentile(df['Sales'], [25, 50, 75])
profit_percentiles = np.percentile(df['Profit'], [25, 50, 75])

print(f"Sales Percentiles: {sales_percentiles}")
print(f"Profit Percentiles: {profit_percentiles}")

#Minimum, Maximum and Range
sales_min = np.min(sales_array)
sales_max = np.max(sales_array)
sales_range = np.ptp(sales_array) #np.ptp calculates from max to min

profit_min = np.min(profit_array)
profit_max = np.max(profit_array)
profit_range = np.ptp(profit_array)

print(f"Sales - Minimum: {sales_min:.2f}, Maximum: {sales_max:.2f}, Range: {sales_range:.2f}")
print(f"Profit - Minimum: {profit_min:.2f}, Maximum: {profit_max:.2f}, Range: {profit_range:.2f}")

# Question 3.2.

#Total Sales, Total Profit and Average Discount by Category
category_analysis = df.groupby('Category').agg({
    'Sales': 'sum',
    'Profit': 'sum',
    'Discount': 'mean'
})
print(category_analysis)

#Total Sales and Profit by Region and Segment
region_segment_analysis = df.groupby(['Region', 'Segment']).agg({
    'Sales': 'sum',
    'Profit': 'sum'
}).reset_index()
print(region_segment_analysis)

#Average Profit Margin by Sub-category (Lowest to Highest)
subcat_analysis = df.groupby('Sub-Category')[['Sales', 'Profit']].sum()
subcat_analysis['Profit Margin (%)'] = (subcat_analysis['Profit'] / subcat_analysis['Sales']) * 100
subcat_sorted = subcat_analysis.sort_values(by='Profit Margin (%)')
print(subcat_sorted['Profit Margin (%)'])

#Monthly Sales Trend
df['Order Date'] = pd.to_datetime(df['Order Date'])
df['Year-Month'] = df['Order Date'].dt.to_period('M')
monthly_sales = df.groupby('Year-Month')['Sales'].sum().reset_index()
monthly_sales['Year-Month'] = monthly_sales['Year-Month'].astype(str)
print(monthly_sales.head())

# Question 3.3.

#Correlation Matrix for numeric column
numeric_cols = ['Sales', 'Quantity', 'Discount', 'Profit']
correlation_matrix = df[numeric_cols].corr()
print(correlation_matrix)

# Question 3.4.

#Filter and display all transactions: Profit
losses_df = df[df['Profit'] < 0]
print(f"\nTotal number of loss-making transaction: {len(losses_df)}")
print(losses_df.head())

#Total Loss Amount + Number of Loss-Making Transactions
total_loss_amount = losses_df['Profit'].sum()
num_loss_transactions = len(losses_df)
print(f"\nNumber of Loss-Making Transactions: {num_loss_transactions}")
print(f"Total Loss Amount: ${abs(total_loss_amount):,.2f}")

#Sub-Category with Highest Total Losses
losses_df = df[df['Profit'] < 0]
subcat_losses = losses_df.groupby('Sub-Category')['Profit'].sum().sort_values()
print("\nTotal Losses by Sub-Category (Sorted from Largest to Smallest Loss):")
print(subcat_losses)
#Finding the single higest loss sub-category
highest_loss_subcat = subcat_losses.index[0]
print(f"\nSub-Category with the highest total loss: {highest_loss_subcat}")
