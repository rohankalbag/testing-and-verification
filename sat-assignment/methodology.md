## Question 1

K = ABC xor BCD 

K = ABC.(B' + C' + D') + (A' + B' + C').BCD

K = ABCD' + A'BCD

K = B.C.(AD' + A'D)

(a)  
- K = B.C.(A + D).(A' + D')
- K' = (A' + B' + C' + D).(A + B' + C' + D')

(b)

```bash
minisat_static q1b.cnf q1b.result > q1b.txt
```

- The expression of K in minisat is done in `q1b.cnf`
- The answer obtained = $(A,B,C,D) \equiv (F, F, F, F)$, found in `q1b.result`

(c) 

```bash
minisat_static q1c.cnf q1c.result > q1c.txt
```

- The expression of K in minisat is done in `q1c.cnf`
- The answer obtained = $(A,B,C,D) \equiv (F, T, T, T)$, found in `q1c.result`
