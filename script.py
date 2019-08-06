import urllib
from bs4 import BeautifulSoup
import HTMLParser

#https://www.nasdaq.com/extended-trading/premarket-mostactive.aspx
tickers_list = []


# Grabs all of the tickers with earnings call today from Yahoo
def GrabStockTickersYahoo():
    url = 'https://finance.yahoo.com/calendar/earnings'
    html = urllib.urlopen(url)
    data_soup = BeautifulSoup(html, 'html.parser')
    string_soup = str(data_soup)

    # Begin to parse data, find number of tickets to watch
    num_tickers_start = string_soup.find('<span data-reactid=\"8\">1-')
    of_string = string_soup.find("of", num_tickers_start)
    results_string = string_soup.find("results", num_tickers_start)
    tickers_substring = string_soup[of_string+3: results_string]
    num_tickers = int(tickers_substring)

    # Collect all of the ticker names
    GrabTickers(url)
    if num_tickers > 100:
        url_100 = url + '?offset=100&size=100'
        GrabTickers(url_100)
    if num_tickers > 200:
        url_200 = url + '?offset=200&size=100'
        GrabTickers(url_200)
    if num_tickers > 300:
        url_300 = url + '?offset=300&size=100'
        GrabTickers(url_300)
    if num_tickers > 400:
        url_400 = url + '?offset=400&size=100'
        GrabTickers(url_400)

    print "Successfully grabbed %d tickers from Yahoo" %(num_tickers)
    return num_tickers


# Find all tickers using {"ticker":"
def GrabTickers(url):
    html = urllib.urlopen(url)
    data_soup = BeautifulSoup(html, 'html.parser')
    string_soup = str(data_soup)
    start_index = string_soup.find('results\":{\"rows\":[{\"ticker')
    data_to_parse = string_soup[start_index: len(string_soup)]
    while "{\"ticker\":\"" in data_to_parse:
        ticker_start = data_to_parse.find("\"ticker\":\"")
        data_to_parse = data_to_parse[ticker_start+10: len(data_to_parse)]
        ticker_end = data_to_parse.find("\"")
        ticker = data_to_parse[0: ticker_end]
        tickers_list.append(ticker)



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


def GrabYahooCallTimes(url):
    html = urllib.urlopen(url)
    data_soup = BeautifulSoup(html, 'html.parser')
    string_soup = str(data_soup)
    string_start = string_soup.find("Pstart(15px) W(20%)")
    string_soup = string_soup[string_start+10: len(string_soup) / 2]
    while "Pstart(15px) W(20%)" in string_soup:
        start_search = string_soup.find("Pstart(15px) W(20%)")
        time_end = string_soup.find("</span>", start_search)
        time_start = string_soup.rfind(">", start_search, time_end)
        call_time = string_soup[time_start+1: time_end]
        string_soup = string_soup[time_end+1: len(string_soup)]
        call_times.append(call_time)



num_tickers = GrabStockTickersYahoo()
call_times = []
GrabYahooCallTimes('https://finance.yahoo.com/calendar/earnings')

if num_tickers > 100:
    GrabYahooCallTimes('https://finance.yahoo.com/calendar/earnings?offset=100&size=100')
if num_tickers > 200:
    GrabYahooCallTimes('https://finance.yahoo.com/calendar/earnings?offset=200&size=100')
if num_tickers > 300:
    GrabYahooCallTimes('https://finance.yahoo.com/calendar/earnings?offset=300&size=100')
if num_tickers > 400:
    GrabYahooCallTimes('https://finance.yahoo.com/calendar/earnings?offset=400&size=100')

print "Successfully grabbed %d call times from Yahoo" %(num_tickers)






# Find additional tickets using the TradingView info
# tradingview_ticket_url = 'https://www.tradingview.com/markets/stocks-usa/earnings/'
# html = urllib.urlopen(tradingview_ticket_url)
# data_soup = BeautifulSoup(html, 'html.parser')
# string_tradingview = str(data_soup)
# f = open("file.txt", 'w')
# f.write(string_tradingview)
# f.close()
# while "href=\"quote/" in string_bloom:
#     ticket_start = string_bloom.find("href=\"quote/")
#     ticket_end = string_bloom.find(":", ticket_start)
#     print(string_bloom[ticket_start+11: ticket_end])

#print (tickers_list)
#print (len(tickers_list))
