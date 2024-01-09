# FI PnL

You can use DV01 * (change in yields) to calculate the approximated P&L, but you really shouldn't do it.

The exact PnL calculation depends on the instruments you're trading. If it's exchange-traded (e.g., futures, futures options), then its price is readily available from the exchange, and the daily change in price should be used for marking to market.

For a bond in general, the daily PnL is (today's clean price + today's accrued interest + coupon payments received today if any) – (yesterday's clean price + yesterday's accrued interest). Oftentimes, government bond trades are financed by repo, so you need to subtract the financing cost from the quantity above, which is roughly the original dirty price (clean price + accrued interest) * repo rate * 1 / 360 (assuming holding period of 1 day and day count convention of Act/360).

For swaps, you'll need to calculate its new market value using the new swap curve. Swaptions are similar – you'll also need to reprice it using the new swap curve & vol cube.