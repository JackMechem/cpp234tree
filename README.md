# Red Black Tree Implementation

#### Comp 282 Project 1

# Usage

## Using Makefile

### Compile

```
make
```

### Run

```
make run
```

## Manually

```
g++ -c main.cpp -o main.o
g++ main.o -o rbtree
```

<details>
  <summary><h1>Expected Output</h1></summary>

```
Tree 1:
(B) 67
  (B) 43
    (R) 27
      (B) 23
        (R) 24
      (B) 38
        (R) 29
        (R) 42
    (R) 47
      (B) 44
      (B) 59
        (R) 61
  (B) 90
    (R) 80
      (B) 69
        (R) 71
      (B) 88
    (B) 95
      (R) 99

Erase 27 -> true
(B) 67
  (B) 43
    (R) 29
      (B) 23
        (R) 24
      (B) 38
        (R) 42
    (R) 47
      (B) 44
      (B) 59
        (R) 61
  (B) 90
    (R) 80
      (B) 69
        (R) 71
      (B) 88
    (B) 95
      (R) 99

Erase 38 -> true
(B) 67
  (B) 43
    (R) 29
      (B) 23
        (R) 24
      (B) 42
    (R) 47
      (B) 44
      (B) 59
        (R) 61
  (B) 90
    (R) 80
      (B) 69
        (R) 71
      (B) 88
    (B) 95
      (R) 99

Erase 44 -> true
(B) 67
  (B) 43
    (R) 29
      (B) 23
        (R) 24
      (B) 42
    (R) 59
      (B) 47
      (B) 61
  (B) 90
    (R) 80
      (B) 69
        (R) 71
      (B) 88
    (B) 95
      (R) 99

Erase 95 -> true
(B) 67
  (B) 43
    (R) 29
      (B) 23
        (R) 24
      (B) 42
    (R) 59
      (B) 47
      (B) 61
  (B) 90
    (R) 80
      (B) 69
        (R) 71
      (B) 88
    (B) 99

Erase 88 -> true
(B) 67
  (B) 43
    (R) 29
      (B) 23
        (R) 24
      (B) 42
    (R) 59
      (B) 47
      (B) 61
  (B) 90
    (R) 71
      (B) 69
      (B) 80
    (B) 99

Erase 59 -> true
(B) 67
  (B) 43
    (R) 29
      (B) 23
        (R) 24
      (B) 42
    (B) 61
      (R) 47
  (B) 90
    (R) 71
      (B) 69
      (B) 80
    (B) 99



Tree 2:
(B) 49
  (B) 17
    (B) 15
      (R) 10
    (R) 34
      (B) 20
        (R) 22
      (B) 41
        (R) 37
        (R) 46
  (B) 76
    (R) 59
      (B) 52
        (R) 57
      (B) 69
        (R) 75
    (R) 97
      (B) 86
        (R) 89
      (B) 99

Erase 99 -> true
(B) 67
  (B) 43
    (R) 29
      (B) 23
        (R) 24
      (B) 42
    (B) 61
      (R) 47
  (B) 90
    (R) 71
      (B) 69
      (B) 80
    (B) 99

Erase 22 -> true
(B) 67
  (B) 43
    (R) 29
      (B) 23
        (R) 24
      (B) 42
    (B) 61
      (R) 47
  (B) 90
    (R) 71
      (B) 69
      (B) 80
    (B) 99

Erase 69 -> true
(B) 67
  (B) 43
    (R) 29
      (B) 23
        (R) 24
      (B) 42
    (B) 61
      (R) 47
  (B) 90
    (R) 71
      (B) 69
      (B) 80
    (B) 99

Erase 15 -> true
(B) 67
  (B) 43
    (R) 29
      (B) 23
        (R) 24
      (B) 42
    (B) 61
      (R) 47
  (B) 90
    (R) 71
      (B) 69
      (B) 80
    (B) 99

Erase 10 -> true
(B) 67
  (B) 43
    (R) 29
      (B) 23
        (R) 24
      (B) 42
    (B) 61
      (R) 47
  (B) 90
    (R) 71
      (B) 69
      (B) 80
    (B) 99

Erase 75 -> true
(B) 67
  (B) 43
    (R) 29
      (B) 23
        (R) 24
      (B) 42
    (B) 61
      (R) 47
  (B) 90
    (R) 71
      (B) 69
      (B) 80
    (B) 99

```

</details>
