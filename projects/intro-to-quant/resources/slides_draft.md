Finance Industry Overview
What is quantitative finance?
"Quant" is short for quantitative
The use of mathematical models, large datasets, and computer algorithms to price assets and manage risk
Assets = anything of value that can be traded
Securities: tradable financial instruments (stocks, bonds)
Commodities: physical goods: hard (gold, oil, must be extracted) or soft (wheat, wool, must be grown)
Derivatives: contracts whose value is derived from an underlying asset (futures, options)
The goal: find "alpha" (market-beating returns) using logic rather than intuition alone
Players in the Quantitative Industry
Three tiers of firms that use quantitative methods.
Alpha Generators. Focus: high-frequency execution, arbitrage, absolute returns
Proprietary trading firms: trade their own capital, no outside clients. Enables extreme technical focus and aggressive profit-sharing. Examples: Optiver, Jane Street, IMC, SIG.
Quantitative hedge funds: manage outside capital (from limited partners) to generate alpha. Range from multi-manager platforms to "black box" systematic funds. Examples: Citadel, Two Sigma, DE Shaw, QRT.
Institutional Giants. Focus: asset allocation, pricing complex derivatives, managing scale
Investment & commercial banks: quants here (often called "Strats") price complex products, run market making desks, and build internal risk engines. Examples: Goldman Sachs, JP Morgan.
Asset managers & insurance: the long game. Optimization to manage trillions in retirement and insurance funds, steady growth and long-term liability matching. Examples: BlackRock, Vanguard.
Enablers. Focus: speed, data integrity, financial infrastructure
Exchanges: the centralised venues where trading happens (covered earlier). They earn revenue through per-trade fees.
Infrastructure providers build the high-performance pipes, cloud environments, and data feeds the industry relies on. Example: Spread Networks built a dedicated fiber from Chicago to New York, ~13ms one-way latency.
Why is This Interesting for Us?
Intellectual challenge: work at the frontier of maths, CS, and economics. Collaborate with (and compete against) IMO medalists, PhDs, and world-class engineers. The problems are non-linear, non-stationary puzzles where the answer changes every millisecond.


Real-time feedback: unlike traditional research, the market gives you a grade on your ideas immediately. Every trading day is a new set of variables. Your code and strategies move real money.


Compensation: total comp for grads at top firms typically ranges from $200k–$400k+ (base + sign-on + bonus). Rapid progression into mid-six and seven-figure territories for high performers. Bonuses tied directly to firm PnL and individual contribution. meritocratic, little red tape.


Scale of success: Citadel Securities generated ~$9.7 billion in net trading revenue in 2024. That's approximately $40 million earned every single trading day. Or ~$6 million revenue per employee.
Trading Games
What they are
Trading games are simulations where participants trade an asset whose true value is unknown. 
The true value is typically derived from some underlying quantity. 
Partial information can be revealed over rounds, and players trade based on their evolving estimate. 
At the end, the true value is revealed and players are scored on final PnL (cash + asset position × true value).
How we play
The format for placing trades varies: open outcry (verbal, shouting quotes), electronic (typed orders into a simulated exchange), or pen and paper. 
Open outcry is a common format in interviews and in training.
Why they matter
Best hands-on way to build intuition for how markets, orderbooks, and spreads actually work
They test estimation under uncertainty, Bayesian updating, spread management, position awareness, and composure under pressure
Appears in interviews at market-making firms, eg. Jane Street and Optiver
Example
Open Outcry Example (optional but maybe good for engagement, time consuming / confusing):
Rules
Any player can shout a quote at any time: 
Bid X: willing to buy at X
Offer X: willing to sell at X
"I'm [bid] at [offer]": meaning they'll buy at the bid price and sell at the offer price.
Any other player can trade against a quote by shouting "yours" (to sell at someone's bid) or "mine" (to buy at someone's offer).
All trades are for 1 unit.
You can only tighten the market. New bids must be higher than the current best bid, new offers must be lower than the current best offer.
Between rounds, new information is revealed and trading continues.
Keep track of your own Pnl
Example markets:
Number of sheep in new zealand 
Distance from sydney to perth in inches
Number of 7-11s in Tokyo
Can release extra information as market develops
Trading Role + Interview Q / sols
Trader Roles
Traders at quant firms sit somewhere on the spectrum between fully systematic and fully manual.
Systematic: adjusting strategies and parameters of automated trading systems. Closer to a research role.
Manual: reacting to news, interpreting macro events, making real-time decisions. Starts with the morning macro meeting.
Day-to-day at an HFT firm:
Morning meeting: overnight news, economic data, strategy adjustments
Adjust autotrader parameters (spread widths, position limits, signal weights)
Monitor live PnL and positions, intervene when needed
Write scripts (usually Python) for analysis and tooling
End-of-day debrief
Interview Process
Online Assessment (mental maths, probability, sequences, aim trainer)
Behavioural Interview (motivations (why trading), typical culture fit questions)
1-3 rounds of technical interviews. Typically 1 on 1 with a trader, goes through probability, brainteasers, market making, etc.
Final round superday. Full day of interviews, content varies from firm to firm. Typically couple technical interviews which are generally more creative and interactive than previous technical rounds (eg. interactive trading games via spreadsheet).  Behaviourals, maybe w head of trading / team leads.
Interview Question
Typically no calculators, pen and paper allowed.


Example Q: dk if difficulty too hard/too easy for time allocated
You have a 10-sided die with faces 0-9. You can roll as many times as you want. Each roll, the value is added to your score. However, if you roll a 0, you go bankrupt. Your score resets to 0 and the game is over. You can choose to stop at any time and keep your current score.
What is the optimal strategy? What is your expected payout?

A:
Development of strategy
Let current bankroll be x.
EV of roll: 1/10 * (-x) + 9/10 * 5
EV of stopping: 0
Take action with higher EV, hence we equate to find optimal stopping point.
1/10 * (-x) + 9/ = 0
x = 45.
Calculation of EV
Hard. Not easy to do precisely without computer simulation, must do long markov chain calculations. Often the case with many problems we encounter in real life trading, hence the ability to arrive at a numerical answer cheaply through estimation is valuable.
Estimation of EV:
Expected number of rolls before stopping = 45/5 = 9.
The probability of making it through 9 rolls is (9/10)^9.
(1 - 1/n)^n is approximately e^-1 for large n. Therefore (9/10)^10 roughly equals e^-1 = 0.37. (9/10)^9 is therefore a little higher ~0.4.
EV is roughly 0.4 * 45 = 18. (probability of success * payout given success)
Dodgy maths, but good enough for an estimation. Actual answer with markov simulation = 17.68.

