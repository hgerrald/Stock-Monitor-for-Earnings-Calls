# Stock-Monitor-for-Earnings-Calls
Still in progress

Tracks all stocks with earnings calls for the given day and alerts for major increases and decreases.
Since python stock API's are no longer available, this program finds the price by scaping HTML.
Since scraping takes too long, it currently works by forkings into 4 processes in C and calling the Python functions.
