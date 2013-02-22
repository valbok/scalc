scalc v0.1
----------

This is a simple mathematical processor that was made as an example of using OOP in tasks like string parsing.
This math processor can calculate expressions like "2+3*4".

Usage:
------
    val@valbok:~/scalc$ ./calc "((sqr( 2+2.212 ) - 29.78/19 + 50 * (8.25^2) + sqrt(5*5))-1000.29  )/24"<br />
    101

It is simplified implementation of <a href="http://www.google.no/search?client=opera&q=29.78+%2B+50+*+88.25%5E5+-+sqr(+sqrt(+27+)+%2B+5+/+25+)&sourceid=opera&ie=utf-8&oe=utf-8&channel=suggest#hl=en&client=opera&channel=suggest&sclient=psy-ab&q=(((+2%2B2.212+)%5E2+-+29.78%2F19+%2B+50+*+(8.25%5E2)+%2B+sqrt(5*5))-1000.29)%2F24&oq=(((+2%2B2.212+)%5E2+-+29.78%2F19+%2B+50+*+(8.25%5E2)+%2B+sqrt(5*5))-1000.29)%2F24&gs_l=serp.3...5123.5835.38.6059.4.4.0.0.0.1.266.639.0j3j1.4.0.les%3B..0.0...1c.1.4.psy-ab.GkSiL1-P3iM&pbx=1&bav=on.2,or.r_gc.r_pw.r_qf.&bvm=bv.42768644,d.bGE&fp=3e3ae3ba58c5360c&biw=1600&bih=770">google calc</a>

Supports folowing actions:
-------------------------
* "-" - Subtraction
* "+" - Adding
* "*" - Multiplication
* "/" - Division
* "^" - Exponentiation
* "sqr()" - Square
* "sqrt()" - Square root

Compilation
-----------
    $ ./compile.sh
