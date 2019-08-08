import urllib
from bs4 import BeautifulSoup
import HTMLParser
import webbrowser
import time



tickers_list = []
#f = open("file.txt", 'w')


# Get stock data
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


# Find tickers using the TradingView info
def GrabTradingViewTickers():
    # Web page to open tradingview link
    #webbrowser.register('chrome', None)
    #webbrowser.open_new('https://www.tradingview.com/markets/stocks-usa/earnings/')
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

    return totalCount


num_tickers = GrabTradingViewTickers()
print("Grabbed %d tickers from TradingView" %(num_tickers))
print tickers_list
# for i in tickers_list:
#     stock_price = GrabStockPrice(i)
#     print (i + " - " + stock_price)
#     f.write(i + " - " + stock_price)
# f.close()
