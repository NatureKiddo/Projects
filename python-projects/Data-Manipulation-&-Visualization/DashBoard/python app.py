import io
import base64
import dash
from dash import Dash, dcc, html, Input, Output
import dash_bootstrap_components as dbc
import pandas as pd
import plotly.express as px
import seaborn as sns
import matplotlib.pyplot as plt


#Loading the Superstore dataset
file_path = r'C:\Users\Revy\Desktop\RichField\Data Manipulation & Visulation\600\Assisgnment\Sample - Superstore.csv'
df = pd.read_csv(file_path, encoding='latin1')

# Question 4

df['Order Date'] = pd.to_datetime(df['Order Date'])
df['YearMonth'] = df['Order Date'].dt.to_period('M').astype(str)

# Helper function for encoding the plots
def fig_to_uri(in_fig, close_fig=True):
    out_img = io.BytesIO()
    in_fig.savefig(out_img, format='png', bbox_inches='tight', facecolor='#060606')
    if close_fig:
        plt.close(in_fig)
    out_img.seek(0)
    encoded = base64.b64encode(out_img.read()).decode("ascii").replace("\n", "")
    return "data:image/png;base64,{}".format(encoded)

#Set global style 
plt.style.use('dark_background')
fig_dist, ax = plt.subplots(figsize=(12, 5))
sns.histplot(df['Profit'], kde=True, ax=ax, color='#00d4ff', bins=100)
ax.axvline(0, color='red', linestyle='--', linewidth=2)
static_plot_1_uri = fig_to_uri(fig_dist) 

#Profit Distribution
fig1_stat, ax1 = plt.subplots(figsize=(12, 5))
sns.histplot(df['Profit'], kde=True, ax=ax1, color='#00d4ff', bins=100)
ax1.axvline(0, color='red', linestyle='--', linewidth=2, label='Profit/Loss Boundary')
ax1.set_title("Static Distribution Analysis: Profit with KDE Overlay", fontsize=18, family='Times New Roman', color='#00d4ff')
ax1.legend()
static_plot_1 = fig_to_uri(fig1_stat)

#Outlier Investigation (Box Plots)
fig2_stat, (ax2a, ax2b) = plt.subplots(1, 2, figsize=(12, 5))
sns.boxplot(y=df['Sales'], ax=ax2a, color='#2980b9')
ax2a.set_title("Sales Outliers", family='Times New Roman', fontsize=16, color='#00d4ff')
sns.boxplot(y=df['Profit'], ax=ax2b, color='#27ae60')
ax2b.set_title("Profit Outliers", family='Times New Roman', fontsize=16, color='#00d4ff')
static_plot_2 = fig_to_uri(fig2_stat)

#Sub-Category Profit/Loss Ranking
fig3_stat, ax3 = plt.subplots(figsize=(12, 6))
sub_profit = df.groupby('Sub-Category')['Profit'].sum().sort_values()
sns.barplot(x=sub_profit.values, y=sub_profit.index, ax=ax3, hue=sub_profit.index, palette="viridis", legend=False)
ax3.axvline(0, color='white', linewidth=1)
ax3.set_title("Total Profit/Loss per Sub-Category", family='Times New Roman', fontsize=18, color='#00d4ff')
static_plot_3 = fig_to_uri(fig3_stat)

subcat_avg_profit = df.groupby('Sub-Category')['Profit'].mean().sort_values()
colors = ['#ff4b4b' if x < 0 else '#2980b9' for x in subcat_avg_profit.values]

fig_avg, ax_avg = plt.subplots(figsize=(12, 7))
sns.barplot(x=subcat_avg_profit.values, y=subcat_avg_profit.index, ax=ax_avg, 
            hue=subcat_avg_profit.index, palette=colors, legend=False)

ax_avg.set_title("Average Profit Efficiency per Sub-Category", fontsize=18, family='Times New Roman', color='#00d4ff')
ax_avg.axvline(0, color='white', linewidth=1.5, linestyle='--')
static_plot_avg_uri = fig_to_uri(fig_avg)

#Correlation Heatmap
fig4_stat, ax4 = plt.subplots(figsize=(10, 8))
#Calculating the correlation matrix
corr_matrix = df[['Sales', 'Quantity', 'Discount', 'Profit']].corr()
sns.heatmap(corr_matrix, annot=True, cmap='RdBu', center=0, ax=ax4, 
            annot_kws={"size": 12, "family": "Times New Roman"})
ax4.set_title("Correlation Heatmap: Sales, Quantity, Discount & Profit", 
             family='Times New Roman', fontsize=18, color='#00d4ff')
static_plot_heatmap = fig_to_uri(fig4_stat)


#The Dash App with Bootstrap CYBORG theme
app = dash.Dash(__name__, external_stylesheets=[dbc.themes.CYBORG], suppress_callback_exceptions=True)

#Professional Layout using DBC
app.layout = dbc.Container(style={'fontFamily': 'Times New Roman'}, fluid=True, children=[
    
#Title - Header of the Dashboard
    dbc.Row([
        dbc.Col([
            html.H1("SuperStore Performance & Profitability Dashboard", 
                    className="text-center mt-4", style={'color': '#00d4ff'}),
            #Student Number
            html.H5("Student Number: [402420531]", 
                    className="text-center", style={'color': '#bbbbbb', 'fontStyle': 'italic'}),
            #Dashboard Description
            html.P("This comprehensive dashboard tracks SuperStore's growth and explores the 'Efficiency Leak' "
                   "by analyzing how regional discounting strategies impact overall bottom-line profitability.", 
                    className="text-center mb-4", style={'color': '#ffffff'})
        ], width=12)
    ]),
    

#Global Filter
    dbc.Row([
        dbc.Col([
            dbc.Card(dbc.CardBody([
                html.Label("Select Region", style={'color': '#00d4ff'}),
                dcc.Dropdown(id='region-filter', options=[{'label': r, 'value': r} for r in df['Region'].unique()],
                             value=df['Region'].unique().tolist(), multi=True, style={'color': '#000'}),
            ]), color="secondary", outline=True)
        ], width=4),
        dbc.Col([
            dbc.Card(dbc.CardBody([
                html.Label("Select Category", style={'color': '#00d4ff'}),
                dcc.Dropdown(id='category-filter', options=[{'label': c, 'value': c} for c in df['Category'].unique()],
                             value=df['Category'].unique().tolist(), multi=True, style={'color': '#000'}),
            ]), color="secondary", outline=True)
        ], width=4),
        dbc.Col([
            dbc.Card(dbc.CardBody([
                html.Label("Select Segment", style={'color': '#00d4ff'}),
                dcc.Dropdown(id='segment-filter', options=[{'label': s, 'value': s} for s in df['Segment'].unique()],
                             value=df['Segment'].unique().tolist(), multi=True, style={'color': '#000'}),
            ]), color="secondary", outline=True)
        ], width=4),
    ], className="mb-4"),
    dbc.Row([ #Date Range Picker
        dbc.Col([
            dbc.Card(dbc.CardBody([
                html.Label("Filter by Order Date Range", style={'color': '#00d4ff', 'fontWeight': 'bold'}),
                dcc.DatePickerRange(
                    id='date-picker',
                    min_date_allowed=df['Order Date'].min().date(),
                    max_date_allowed=df['Order Date'].max().date(),
                    start_date=df['Order Date'].min().date(),
                    end_date=df['Order Date'].max().date(),
                    style={'backgroundColor': '#333', 'color': 'white'}
                ),
            ]), color="secondary", outline=True, className="mb-4")
        ], width=12)
    ]),

#Main Tabs System
    dbc.Tabs([

#Tab 1
        dbc.Tab(label="Overview", children=[
            html.Br(),
            dbc.Row([
                dbc.Col(dbc.Card(dbc.CardBody([html.H4("Total Sales"), html.P(id='total-sales-kpi')]), outline=True), width=4),
                dbc.Col(dbc.Card(dbc.CardBody([html.H4("Total Profit"), html.P(id='total-profit-kpi')]), outline=True), width=4),
                dbc.Col(dbc.Card(dbc.CardBody([html.H4("Avg Profit Margin"), html.P(id='avg-margin-kpi')]), outline=True), width=3),
                dbc.Col(dbc.Card(dbc.CardBody([html.H4("Number of Orders"), html.P(id='num-orders-kpi')]), outline=True), width=3),
            ]),
            html.Br(),
            dbc.Row([
                dbc.Col(dcc.Graph(id='discount-profit-scatter'), width=12)
            ])
        ]),
#Tab 2
        dbc.Tab(label="Product Analysis", children=[
            html.Br(),
            dbc.Row([
                dbc.Col(dcc.Graph(id='category-profit-graph'), width=6),
                dbc.Col(dcc.Graph(id='sales-profit-comp-graph'), width=6)
            ])
        ]),

#Tab 3
        dbc.Tab(label="Temporal Trends", children=[
            html.Br(),
            dbc.Row([
               dbc.Col(dcc.Graph(id='sales-trend-graph'), width=12)
            ])
        ]),
#Tab 4
        dbc.Tab(label="Advanced Analytics", children=[
            html.Br(),
            
            #Profit Distribution
            dbc.Row([
                dbc.Col([
                    html.H4("1. Profit Distribution Analysis", className="text-center"),
                    html.Img(src=static_plot_1, style={'width': '100%'}),
                    html.P("Analysis: This histogram shows that most transactions fall near the zero-profit mark. "
                        "The red line highlights the critical boundary where transactions flip from profitable to losses.", 
                        style={'fontStyle': 'italic', 'color': '#bbbbbb', 'padding': '10px', 'textAlign': 'center'})
                ], width=8) 
            ], justify="center", className="mb-5"), 

            #Sub-Category Efficiency
            dbc.Row([
                dbc.Col([
                    html.H4("2. Average Profit Efficiency by Sub-Category", className="text-center"),
                    html.Img(src=static_plot_avg_uri, style={'width': '100%'}),
                    html.P("Analysis: This comparison identifies which products are consistent 'leaks.' "
                        "Sub-categories in red (like Tables) show that, on average, every sale results in a loss.", 
                        style={'fontStyle': 'italic', 'color': '#bbbbbb', 'padding': '10px', 'textAlign': 'center'})
                ], width=8)
            ], justify="center", className="mb-5"),

            #Correlation Heatmap
            dbc.Row([
                dbc.Col([
                    html.H4("3. Numeric Variable Correlations", className="text-center"),
                    html.Img(src=static_plot_heatmap, style={'width': '100%'}),
                    html.P("Analysis: The heatmap reveals the strength of relationships between variables. "
                        "A strong negative correlation between Discount and Profit often confirms the 'Efficiency Leak' theory.", 
                        style={'fontStyle': 'italic', 'color': '#bbbbbb', 'padding': '10px', 'textAlign': 'center'})
                ], width=6)
            ], justify="center")
        ]),
    ])
])

#Interactivity Callback
#This decorator connects the UI inputs (Dropdowns/Date Picker) to the chart functions
@app.callback(
    [Output('sales-trend-graph', 'figure'),
     Output('category-profit-graph', 'figure'),
     Output('sales-profit-comp-graph', 'figure'),
     Output('discount-profit-scatter', 'figure'),
     Output('total-sales-kpi', 'children'),
     Output('total-profit-kpi', 'children'),
     Output('avg-margin-kpi', 'children'),
     Output('num-orders-kpi', 'children')],
    [Input('region-filter', 'value'),
     Input('category-filter', 'value'),
     Input('segment-filter', 'value'),
     Input('date-picker', 'start_date'),
     Input('date-picker', 'end_date')]
)
def update_graphs(regions, categories, segments, start_date, end_date):
    #If any filter is cleared entirely, return a blank slate instead of crashing
    if not regions or not categories or not segments or not start_date or not end_date:
        #Return empty figures and zeroed KPIs
        empty_fig = px.scatter(title="No data selected").update_layout(template="plotly_dark")
        return empty_fig, empty_fig, empty_fig, empty_fig, "$0.00", "$0.00", "0.00%", "0"

 #STEP 1: Apply temporal filtering using the Date Picker range
    mask = (df['Order Date'] >= start_date) & (df['Order Date'] <= end_date)
    date_filtered_df = df.loc[mask]
#STEP 2: Apply categorical filtering for Region, Category, and Segment
#The .isin() method allows for multi-select filtering
    filtered_df = date_filtered_df[
                    (df['Region'].isin(regions)) &
                    (df['Category'].isin(categories)) &
                    (df['Segment'].isin(segments))
                    ]
    #Handle the "Empty Dataframe" edge case for KPIs
    if filtered_df.empty:
        return empty_fig, empty_fig, empty_fig, empty_fig, "$0.00", "$0.00", "0.00%", "0"


#STEP 3: Dynamic KPI Calculations
#Formatting numbers with dollar signs, commas, and percentage symbols for readability
    sales_val = f"${filtered_df['Sales'].sum():,.2f}"
    profit_val = f"${filtered_df['Profit'].sum():,.2f}"
    total_sales_sum = filtered_df['Sales'].sum()
    total_profit_sum = filtered_df['Profit'].sum()
    #Avoid division by zero if filters return no data
    margin_val = f"{(total_profit_sum / total_sales_sum):.2%}" if total_sales_sum != 0 else "0.00%"
    #Count unique Order IDs to represent total distinct orders
    orders_val = f"{filtered_df['Order ID'].nunique():,}"

#STEP 4: CHARTS
#Chart 1: Monthly Sales Trend 
    trend_data = filtered_df.groupby('YearMonth')['Sales'].sum().reset_index()
    fig1 = px.line(trend_data, x='YearMonth', y='Sales', 
                   title="Monthly Sales Performance Over Time", markers=True, template="plotly_dark")
    fig1.update_xaxes(rangeslider_visible=True,
                      rangeselector=dict(buttons=list([
                          dict(count=6, label="6 Months", step="month", stepmode="backward"),
                          dict(count=1, label="1 Year", step="year", stepmode="backward"),
                          dict(step="all", label="All Time")
                      ]),
                      bgcolor="#333333", font=dict(color="white")
                )
        )

#Chart 2: Profit by Category
    cat_data = filtered_df.groupby(['Category', 'Sub-Category'])['Profit'].sum().reset_index()
    cat_data = cat_data.sort_values(by='Profit', ascending=False)
    fig2 = px.bar(cat_data, x='Sub-Category', y='Profit', color='Category', 
                  title="Profitability Breakdown by Product Group", template="plotly_dark",
                  barmode='group')
    fig2.add_hline(y=0, line_color='white', line_width=1)
    fig2.update_layout(xaxis_tickangle=-45)
#Melting data for the Sales vs Profit
    comp_data = filtered_df.groupby('Category')[['Sales', 'Profit']].sum().reset_index()
    comp_data_melted = comp_data.melt(id_vars='Category', var_name='Metric', value_name='Amount')
    fig4 = px.bar(comp_data_melted, x='Category', y='Amount', color='Metric',
                  barmode='group', 
                  color_discrete_map={'Sales': '#00d4ff',
                  'Profit': '#27ae60'},
                  title="Sales vs Profit Performance by Category",
                  template="plotly_dark"
                  )

#Chart 3: Sales vs Profit 
    fig3 = px.scatter(filtered_df, x='Sales', y='Profit', color='Segment',
                     size='Quantity', hover_name='Product Name', 
                     hover_data={'Sales': ':,.2f',
                                 'Profit': ':,.2f',
                                 'Category': True,
                                 'Sub-Category': True,
                                 'Discount': ':,.2f',
                                 'Region': True},
                     title="Interactive Analysis: Sales vs Profit", template="plotly_dark")
    fig3.add_hline(y=0, line_dash="dash", line_color="red")
    fig3.update_layout(dragmode='lasso')
#STEP 5: Global Formatting
#Applying uniform styling to all Plotly objects to match the Cyborg theme   
    for fig in [fig1, fig2, fig3, fig4]:
        fig.update_layout(
            title_x=0.5,          
            title_font_size=24,
            font_family="Times New Roman",
            title_font_family="Times New Roman",
            template="plotly_dark",
            #This makes the charts blend into the dashboard background
            paper_bgcolor='rgba(0,0,0,0)', 
            plot_bgcolor='rgba(0,0,0,0)',
            #Makes it don't look so not together?
            margin=dict(l=40, r=40, t=60, b=40) 
        )
#Return order must strictly follow the Output list defined in the decorator
    return fig1, fig2, fig4, fig3, sales_val, profit_val, margin_val, orders_val

if __name__ == '__main__':
    app.run(debug=True)