import urllib
from bs4 import BeautifulSoup
import HTMLParser
import webbrowser
import time


tickers_list = []

# Finds the stock price of a given ticker. Takes a while to run...
def GrabStockPrice(symbol):
    try:
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


    except:
        stock_price = "N/A"

    return stock_price


# Find all tickers with earnings calls today from the TradingView website
def GrabTradingViewTickers():
    # Web page to open tradingview link
    webbrowser.register('chrome', None)
#    webbrowser.open_new('https://www.tradingview.com/markets/stocks-usa/earnings/')
    time.sleep(1)
    response = raw_input("\nDid you load all tickers and save the page as \"earnings.html\" in the same directory as this program? [y]/[n]:  ")

    if response != 'y':
        sys.exit("Can't listen to instructions? Right click and save as!")

    f = open("earnings.html", 'r')
    tradingview_data = f.read()
    f.close()

    # Find the total number of tickers
    totalCount_start = tradingview_data.find("totalCount")
    totalCount_end = tradingview_data.find(",", totalCount_start)
    totalCount = int(tradingview_data[totalCount_start+13: totalCount_end])

    # Find the stock tickers
    search_for = "table__result-row\" data-symbol"
    stock_start = 0
    for i in range (0, totalCount):
        search_start = tradingview_data.find(search_for, stock_start)
        stock_start = tradingview_data.find(":", search_start)
        stock_end = tradingview_data.find("\">", stock_start)
        stock = tradingview_data[stock_start+1: stock_end]
        tickers_list.append(stock)

    print "Successfully grabbed %d tickers from TradingView" %(totalCount)
    return totalCount


# Open the live chart of a given stock ticker
def OpenTicker(symbol):
        webbrowser.register('chrome', None)
        url = 'https://www.marketwatch.com/investing/stock/' + symbol
        webbrowser.open_new(url)
        time.sleep(1)


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
    duplicate_count = 0

    # Collect all of the ticker names
    duplicate_count -= GrabTickers(url)
    if num_tickers > 100:
        url_100 = url + '?offset=100&size=100'
        duplicate_count -= GrabTickers(url_100)
    if num_tickers > 200:
        url_200 = url + '?offset=200&size=100'
        duplicate_count -= GrabTickers(url_200)
    if num_tickers > 300:
        url_300 = url + '?offset=300&size=100'
        duplicate_count -= GrabTickers(url_300)
    if num_tickers > 400:
        url_400 = url + '?offset=400&size=100'
        duplicate_count -= GrabTickers(url_400)

    print "Successfully grabbed %d tickers from Yahoo" %(num_tickers + duplicate_count)
    return num_tickers + duplicate_count



# Find all tickers using {"ticker":" --- Helper function for finding Yahoo tickers
def GrabTickers(url):
    dup_count = 0
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
        flag = 0
        for i in tickers_list:
          if ticker == i:
            flag = 1
            dup_count = dup_count + 1
        if flag == 0:
          tickers_list.append(ticker)
    return dup_count


def FindStocksWithEarningsCalls():
    newfile = open("tickers.txt", "w+")
    num_tradingview = GrabTradingViewTickers()
    num_yahoo = GrabStockTickersYahoo()
    totalTickers = num_tradingview + num_yahoo
    newfile.write("%d\n" % (totalTickers))
    for i in tickers_list:
        newfile.write(i)
        newfile.write("\n")
    newfile.close()

    print "Successfully grabbed %d total tickers (took out duplicates)" %(totalTickers)
