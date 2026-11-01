#Data Loading : 402420531
import numpy as np  
import pandas as pd
import matplotlib as plt   
import seaborn as sns   

#Loading the Superstore dataset
file_path = r'C:\Users\Revy\Desktop\RichField\Data Manipulation & Visulation\600\Assisgnment\Sample - Superstore.csv'
df = pd.read_csv(file_path, encoding='latin1')

# QUESTION 1.1)

#Displaying the first 10 rows
print("---- First 10 Rows ----")
print(df.head(10))

#Displaying the last 5 rows
print("\n---- Last 5 Rows ----")
print(df.tail(5))

#Shape of dataset (Rows x Columns)
print("\n---- Dataset Shape ----")
print(df.shape)

# QUESTION 1.2)

#Display column names, data types and non-null counts
print("\n---- Dataset Information ----")
print("\n---Column Names---")
print(df.columns.tolist())
print("\n---Data Types---")
print(df.dtypes)
print("\n---Non-null Counts---")
print(df.count())

#Generate Summary statistics for ALL columns
print("\n---- Summary Statistics ----")
print(df.describe(include='all'))

#Identify and list all unique values for:Ship Mode, Segment, Region, Category
print("\n---- Unique Values ----")
print(f"Ship Mode: {df['Ship Mode'].unique()}")
print(f"Segment: {df['Segment'].unique()}")
print(f"Region: {df['Region'].unique()}")
print(f"Category: {df['Category'].unique()}")
