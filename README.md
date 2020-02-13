# Stock-Monitor-for-Earnings-Calls

## What it Does
Many of the top gaining stocks each day are those that report positive earnings calls. Since I can't listen to each one, this program tracks each for sudden jumps, indicating a positive report. If one rises quickly, this program will find it (hopefully as it is still on the rise).


## How it Works
Since most stock APIs come with a price, *stock_functions.py* contains Python code that scrapes the HTML of stock tickers with earnings calls that day. After scraping the HTML, it parses the code to find the strings of each ticker and save them in a list. It also scrapes HTML to find the prices of each stock. HTML scraping, however, is a time consuming process and would take a significant amount of time to loop through and find prices of 200+ stocks. A stock will plateau very soon after reporting a positive earnings calls, so it is critical to purchase it as early as possible.

To combat the slow speed, the C code splits the workload into either threads or processes, calls the embedded Python code, and compares the live prices with the original at the start of the program. Threads in Python don't really work, and even in C when embedding Python can be troublesome. They can only run concurrently to a certain extent since each is fighting over the GIL (at least from what I understand from my brief reading online).

The processes option usually runs faster but took longer to setup since processes don't typically share memory. I used mmap to allow the processes to share memory for a few specific variables to combat this issue. 

## Using the Program
Sadly the program no longer runs as MarketWatch is now able to detect I am a bot. I will hopefully be able to find a way around this when I get more free time :(

![bot](https://github.com/hgerrald/Stock-Monitor-for-Earnings-Calls/blob/master/bot.png?raw=true)
