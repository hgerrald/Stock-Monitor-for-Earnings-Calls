import urllib
from bs4 import BeautifulSoup
import HTMLParser

# Grab HTML
#url = 'https://finance.yahoo.com/calendar/earnings'
url = 'https://finance.yahoo.com/calendar/earnings?from=2019-07-28&to=2019-08-03&day=2019-08-02'
html = urllib.urlopen(url)
data_soup = BeautifulSoup(html, 'html.parser')

# Begin to parse data
string_soup = str(data_soup)
start_search = 'results\":{\"rows":[{\"ticker'
start_index = string_soup.find(start_search)
data_to_parse = string_soup[start_index: len(string_soup)]

# Find all tickers using {"ticker":"
tickets_list = []
while "{\"ticker\":\"" in data_to_parse:
    ticket_start = data_to_parse.find("\"ticker\":\"")
    data_to_parse = data_to_parse[ticket_start+10: len(data_to_parse)]
    ticket_end = data_to_parse.find("\"")
    ticket = data_to_parse[0: ticket_end]
    tickets_list.append(ticket)

# Get stock data
def GrabStockData(symbol):
    stock_url = 'https://www.marketwatch.com/investing/stock/' + symbol
    html = urllib.urlopen(stock_url)
    stock_html_data = BeautifulSoup(html, 'html.parser')
    string_html_data = str(stock_html_data)
    start_search = string_html_data.find('<sup class=\"character\">$</sup>')
  # About 10% - 20% have differnt HTML code
    if string_html_data[start_search + 32: start_search + 36] == "span":
        span = string_html_data.find("span", start_search)
        start_stock = string_html_data.find(">", span)
        end_stock = string_html_data.find("<", span)
        stock_price = string_html_data[start_stock+1: end_stock]

    else:
        end_search = string_html_data.index('</bg-quote>', start_search)
        small_string = string_html_data[start_search: end_search]
        start_stock = small_string.rfind('>', 0, len(small_string))
        stock_price = small_string[start_stock+1: end_search]

    print (symbol + " - " + stock_price)

for i in tickets_list:
    GrabStockData(i)
