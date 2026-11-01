import numpy as np  
import pandas as pd
import matplotlib.pyplot as plt   
import seaborn as sns   

#Loading the Superstore dataset
file_path = r'C:\Users\Revy\Desktop\RichField\Data Manipulation & Visulation\600\Assisgnment\Sample - Superstore.csv'
df = pd.read_csv(file_path, encoding='latin1')

# QUESTION 2.1)

#Missing values per Column
missing_values_count = df.isnull().sum()
print("---- Missing Values ----")
print(missing_values_count)

#Percentage of missing value 
missing_percentage = (df.isnull().sum() / len(df)) * 100
print("\n---- Missing Percentage ----")
print(missing_percentage)

#Missing values patterns in Seaborn
plt.figure(figsize=(12,6))
sns.heatmap(df.isnull(), yticklabels=False, cbar=False, cmap='Blues')
plt.title('Missing Value Pattern (Blue indicated Missing Data)')
plt.show()

# QUESTION 2.2)

#Counting duplicate rows on ALL columns
duplicate_count = df.duplicated().sum()
print(f"Total number of duplicate rows: {duplicate_count}")

#Displaying ORDER IDs dupilcates
multi_item_id = df.duplicated(subset=['Order ID'], keep=False)
#Filitering for Legitimate multi rows
multi_item_df = df[multi_item_id]
print(f"Total rows belonging to multi-item orders: {len(multi_item_df)}")
print(f"Unique orders with multiple items: {df[df.duplicated(subset=['Order ID'])]['Order ID'].nunique()}")

#Removing the true duplicated rows
initial_rows = len(df)
df.drop_duplicates(subset=['Order ID', 'Product Name', 'Sales', 'Quantity', 'Order Date'], keep='first', inplace=True)
#Calculate how many were removed.
removed_count = initial_rows - len(df)
print(f"Total Rows Removed: {removed_count}")
print(f"Final Dataset Size: {len(df)} rows")

#UNCOMMENT THE CODE TO RUN IT. I PUT IT HERE IN COMMENTS INCASE YOU NEED IT.
#Check for exact product matches within the same ORDER ID
#I am going exclude 'Row ID' because it is unique by design
#potential_errors = df.duplicated(subset=['Order ID', 'Product Name', 'Sales', 'Quantity'], keep=False)

#See these potential accidental entries
#error_check_df = df[potential_errors].sort_values(by='Order ID')
#print("Potential accidental duplicates within orders:")
#print(error_check_df[['Order ID', 'Product Name', 'Sales', 'Quantity']].head())

# QUESTION 2.3)

#Covert to datetime objects
df['Order Date'] = pd.to_datetime(df['Order Date'])
df['Ship Date'] = pd.to_datetime(df['Ship Date'])

print(df[['Order Date', 'Ship Date']].dtypes)

#Creating a New Column : Shipping Days
df['Shipping_Days'] = (df['Ship Date'] - df['Order Date']).dt.days
print(df[['Order Date', 'Ship Date', 'Shipping_Days']].head())
print(f"\nData type pf Shipping_Days: {df['Shipping_Days'].dtype}")

#Creating a New Column : Profit Margin
df['Profit_Margin'] = (df['Profit'] / df['Sales']) * 100
print(df[['Sales', 'Profit', 'Profit_Margin']].head())

#Extract Year and Month from Order Date
df['Order_Year'] = df['Order Date'].dt.year
df['Order_Month'] = df['Order Date'].dt.month
print(df[['Order Date', 'Order_Year', 'Order_Month']].head())

# QUESTION 2.4)

#IQR for the Sales, Profit and Discount columns
cols = ['Sales', 'Profit', 'Discount']
#Q1 (25 percentile) and Q3 (75 percentile)
Q1 = df[cols].quantile(0.25)
Q3 = df[cols].quantile(0.75)
#Calculating the IQR
IQR = Q3 - Q1
print("---- Interquartile Range (IQR) ----")
print(IQR)

#Outliners with the 1.5xIQR method
numeric_cols = ['Sales', 'Quantity', 'Discount', 'Profit', 'Shipping_Days', 'Profit_Margin']

for col in numeric_cols:
    Q1 = df[col].quantile(0.25)
    Q3 = df[col].quantile(0.75)
    IQR = Q3 - Q1

    lower_bound = Q1 - 1.5 * IQR
    upper_bound = Q3 + 1.5 * IQR

    outliers = df[(df[col] < lower_bound) | (df[col] > upper_bound)]

    print(f"{col}: Found {len(outliers)} outliers")
    print(f"\nBounds: [{lower_bound:.2f}, {upper_bound:.2f}]\n")

#Creating a box plots showing the ditribution of Sales and Profit
sns.set_theme(style="whitegrid")
#Figure with 2 subplots 
fig, axes = plt.subplots(1, 2, figsize=(15,6))
#Box plot for Sales
sns.boxplot(y=df['Sales'], ax=axes[0], color='skyblue', width=0.5)
axes[0].set_title('Distribution of Sales (with Outliers)', fontsize=14)
axes[0].set_ylabel('Sales Value ($)')
#Box plot for Profit
sns.boxplot(y=df['Profit'], ax=axes[1], color='salmon', width=0.5)
axes[1].set_title('Distribution of Profit (with Outliers)', fontsize=14)
axes[1].set_ylabel('Profit Value ($)')
#Main title for the figure
plt.suptitle('Outlier Identification: Sales and Profit', fontsize=16)
#Preventing the overlapping
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
plt.show()