import io
import base64
import dash
from dash import Dash, dcc, html, Input, Output
import dash_bootstrap_components as dbc
import pandas as pd
import plotly.express as px
import seaborn as sns
import matplotlib.pyplot as plt

# 1. Loading the Superstore dataset
# Ensure 'Sample - Superstore.csv' is in the same folder as this app.py file
file_path = 'Sample - Superstore.csv'
df = pd.read_csv(file_path, encoding='latin1')

# 2. Data Preparation
df['Order Date'] = pd.to_datetime(df['Order Date'])
df['YearMonth'] = df['Order Date'].dt.to_period('M').astype(str)

# Helper function for encoding static plots into base64 for Dash embedding
def fig_to_uri(in_fig, close_fig=True):
    out_img = io.BytesIO()
    in_fig.savefig(out_img, format='png', bbox_inches='tight', facecolor='#060606')
    if close_fig:
        plt.close(in_fig)
    out_img.seek(0)
    encoded = base64.b64encode(out_img.read()).decode("ascii").replace("\n", "")
    return "data:image/png;base64,{}".format(encoded)

# 3. Generating Static Plots (Seaborn/Matplotlib)
plt.style.use('dark_background')

# Static Plot 1: Profit Distribution
fig1_stat, ax1 = plt.subplots(figsize=(12, 5))
sns.histplot(df['Profit'], kde=True, ax=ax1, color='#00d4ff', bins=100)
ax1.axvline(0, color='red', linestyle='--', linewidth=2, label='Profit/Loss Boundary')
ax1.set_title("Static Distribution Analysis: Profit with KDE Overlay", fontsize=18, family='Times New Roman', color='#00d4ff')
ax1.legend()
static_plot_1 = fig_to_uri(fig1_stat)

# Static Plot 2: Average Profit Efficiency
subcat_avg_profit = df.groupby('Sub-Category')['Profit'].mean().sort_values()
colors = ['#ff4b4b' if x < 0 else '#2980b9' for x in subcat_avg_profit.values]
fig_avg, ax_avg = plt.subplots(figsize=(12, 7))
sns.barplot(x=subcat_avg_profit.values, y=subcat_avg_profit.index, hue=subcat_avg_profit.index, palette=colors, ax=ax_avg, legend=False)
ax_avg.set_title("Average Profit Efficiency per Sub-Category", fontsize=18, family='Times New Roman', color='#00d4ff')
ax_avg.axvline(0, color='white', linewidth=1.5, linestyle='--')
static_plot_avg_uri = fig_to_uri(fig_avg)

# Static Plot 3: Correlation Heatmap
fig_heat, ax_heat = plt.subplots(figsize=(10, 8))
corr_matrix = df[['Sales', 'Quantity', 'Discount', 'Profit']].corr()
sns.heatmap(corr_matrix, annot=True, cmap='RdBu', center=0, ax=ax_heat)
ax_heat.set_title("Correlation Heatmap: Sales, Quantity, Discount & Profit", family='Times New Roman', fontsize=18, color='#00d4ff')
static_plot_heatmap = fig_to_uri(fig_heat)

# 4. Initialize Dash App with CYBORG theme
app = dash.Dash(__name__, external_stylesheets=[dbc.themes.CYBORG], suppress_callback_exceptions=True)

# 5. App Layout
app.layout = dbc.Container(style={'fontFamily': 'Times New Roman'}, fluid=True, children=[
    # Header Section
    dbc.Row([
        dbc.Col([
            html.H1("SuperStore Performance & Profitability Dashboard", className="text-center mt-4", style={'color': '#00d4ff'}),
            html.H5("Student Number: [402420531]", className="text-center", style={'color': '#bbbbbb', 'fontStyle': 'italic'}),
            html.P("Analyzing regional discounting strategies and bottom-line profitability.", 
                   className="text-center mb-4", style={'color': '#ffffff'})
        ], width=12)
    ]),

    # Global Filters Row
    dbc.Row([
        dbc.Col([
            dbc.Card(dbc.CardBody([
                html.Label("Select Region", style={'color': '#00d4ff'}),
                dcc.Dropdown(id='region-filter', options=[{'label': r, 'value': r} for r in df['Region'].unique()],
                             value=df['Region'].unique().tolist(), multi=True, style={'color': '#000'})
            ]), color="secondary", outline=True)
        ], width=3),
        dbc.Col([
            dbc.Card(dbc.CardBody([
                html.Label("Select Category", style={'color': '#00d4ff'}),
                dcc.Dropdown(id='category-filter', options=[{'label': c, 'value': c} for c in df['Category'].unique()],
                             value=df['Category'].unique().tolist(), multi=True, style={'color': '#000'})
            ]), color="secondary", outline=True)
        ], width=3),
        dbc.Col([
            dbc.Card(dbc.CardBody([
                html.Label("Select Segment", style={'color': '#00d4ff'}),
                dcc.Dropdown(id='segment-filter', options=[{'label': s, 'value': s} for s in df['Segment'].unique()],
                             value=df['Segment'].unique().tolist(), multi=True, style={'color': '#000'})
            ]), color="secondary", outline=True)
        ], width=3),
        dbc.Col([
            dbc.Card(dbc.CardBody([
                html.Label("Date Range", style={'color': '#00d4ff'}),
                dcc.DatePickerRange(id='date-picker', min_date_allowed=df['Order Date'].min().date(),
                                    max_date_allowed=df['Order Date'].max().date(),
                                    start_date=df['Order Date'].min().date(), end_date=df['Order Date'].max().date())
            ]), color="secondary", outline=True)
        ], width=3),
    ], className="mb-4"),

    # Tabs for Content Organization
    dbc.Tabs([
        dbc.Tab(label="Overview", children=[
            html.Br(),
            dbc.Row([
                dbc.Col(dbc.Card(dbc.CardBody([html.H4("Total Sales"), html.P(id='total-sales-kpi')]), outline=True), width=3),
                dbc.Col(dbc.Card(dbc.CardBody([html.H4("Total Profit"), html.P(id='total-profit-kpi')]), outline=True), width=3),
                dbc.Col(dbc.Card(dbc.CardBody([html.H4("Avg Profit Margin"), html.P(id='avg-margin-kpi')]), outline=True), width=3),
                dbc.Col(dbc.Card(dbc.CardBody([html.H4("Number of Orders"), html.P(id='num-orders-kpi')]), outline=True), width=3),
            ]),
            html.Br(),
            dbc.Row([dbc.Col(dcc.Graph(id='discount-profit-scatter'), width=12)])
        ]),
        dbc.Tab(label="Analysis", children=[
            html.Br(),
            dbc.Row([
                dbc.Col(dcc.Graph(id='category-profit-graph'), width=6),
                dbc.Col(dcc.Graph(id='sales-profit-comp-graph'), width=6)
            ])
        ]),
        dbc.Tab(label="Temporal Trends", children=[
            html.Br(),
            dbc.Row([
                # Add this missing line back in:
                dbc.Col(dcc.Graph(id='sales-trend-graph'), width=12)
            ])
        ]),
        dbc.Tab(label="Advanced Analytics", children=[
            html.Br(),
            dbc.Row([
                dbc.Col([html.H4("1. Profit Distribution", className="text-center"), html.Img(src=static_plot_1, style={'width': '100%'}), html.P("Analysis: This histogram shows that most transactions fall near the zero-profit mark. ""The red line highlights the critical boundary where transactions flip from profitable to losses.", style={'fontStyle': 'italic', 'color': '#bbbbbb', 'padding': '10px', 'textAlign': 'center'})], width=8),
            ], justify="center", className="mb-5"),
            dbc.Row([
                dbc.Col([html.H4("2. Profit Efficiency", className="text-center"), html.Img(src=static_plot_avg_uri, style={'width': '100%'}), html.P("Analysis: This comparison identifies which products are consistent 'leaks.' ""Sub-categories in red (like Tables) show that, on average, every sale results in a loss.", style={'fontStyle': 'italic', 'color': '#bbbbbb', 'padding': '10px', 'textAlign': 'center'})], width=8),
            ], justify="center", className="mb-5"),
            dbc.Row([
                dbc.Col([html.H4("3. Correlation Heatmap", className="text-center"), html.Img(src=static_plot_heatmap, style={'width': '100%'}), html.P("Analysis: The heatmap reveals the strength of relationships between variables. ""A strong negative correlation between Discount and Profit often confirms the 'Efficiency Leak' theory.", style={'fontStyle': 'italic', 'color': '#bbbbbb', 'padding': '10px', 'textAlign': 'center'})], width=6),
            ], justify="center")
        ]),
    ])
])

# 6. Interactivity Callbacks
@app.callback(
    [Output('sales-trend-graph', 'figure'), Output('category-profit-graph', 'figure'),
     Output('sales-profit-comp-graph', 'figure'), Output('discount-profit-scatter', 'figure'),
     Output('total-sales-kpi', 'children'), Output('total-profit-kpi', 'children'),
     Output('avg-margin-kpi', 'children'), Output('num-orders-kpi', 'children')],
    [Input('region-filter', 'value'), Input('category-filter', 'value'),
     Input('segment-filter', 'value'), Input('date-picker', 'start_date'),
     Input('date-picker', 'end_date')]
)
def update_graphs(regions, categories, segments, start_date, end_date):
    # Defensive check for empty inputs
    if not all([regions, categories, segments, start_date, end_date]):
        empty_fig = px.scatter(title="No data selected").update_layout(template="plotly_dark")
        return [empty_fig]*4 + ["$0.00", "$0.00", "0.00%", "0"]

    # Filter Data
    mask = (df['Order Date'] >= start_date) & (df['Order Date'] <= end_date)
    filtered_df = df.loc[mask]
    filtered_df = filtered_df[(filtered_df['Region'].isin(regions)) & (filtered_df['Category'].isin(categories)) & (filtered_df['Segment'].isin(segments))]

    if filtered_df.empty:
        empty_fig = px.scatter(title="No matches found").update_layout(template="plotly_dark")
        return [empty_fig]*4 + ["$0.00", "$0.00", "0.00%", "0"]

    # KPI Calculations
    t_sales = filtered_df['Sales'].sum()
    t_profit = filtered_df['Profit'].sum()
    sales_val, profit_val = f"${t_sales:,.2f}", f"${t_profit:,.2f}"
    margin_val = f"{(t_profit / t_sales):.2%}" if t_sales != 0 else "0.00%"
    orders_val = f"{filtered_df['Order ID'].nunique():,}"

    # Visualizations
    fig1 = px.line(filtered_df.groupby('YearMonth')['Sales'].sum().reset_index(), x='YearMonth', y='Sales', title="Monthly Sales Trend", markers=True)
    fig1.update_xaxes(rangeslider_visible=True)

    fig2 = px.bar(filtered_df.groupby(['Category', 'Sub-Category'])['Profit'].sum().reset_index(), x='Sub-Category', y='Profit', color='Category', title="Sub-Category Profitability")
    
    comp_melted = filtered_df.groupby('Category')[['Sales', 'Profit']].sum().reset_index().melt(id_vars='Category', var_name='Metric', value_name='Amount')
    fig4 = px.bar(comp_melted, x='Category', y='Amount', color='Metric', barmode='group', title="Sales vs Profit")

    fig3 = px.scatter(filtered_df, x='Sales', y='Profit', color='Segment', size='Quantity', hover_name='Product Name', title="Transaction Detail")
    fig3.add_hline(y=0, line_dash="dash", line_color="red")

    for f in [fig1, fig2, fig4, fig3]:
        f.update_layout(title_x=0.5, template="plotly_dark", paper_bgcolor='rgba(0,0,0,0)', plot_bgcolor='rgba(0,0,0,0)')

    return fig1, fig2, fig4, fig3, sales_val, profit_val, margin_val, orders_val

if __name__ == '__main__':
    app.run(debug=True)
