(define (problem warehouse10x10) (:domain warehouse)
    (:objects 
        x1 x2 x3 x4 x5 x6 x7 x8 x9 x10  - position
        y1 y2 y3 y4 y5 y6 y7 y8 y9 y10  - position
        r1 r2 r3 - robot
        A B C D E F G - item
    )

    (:init
        ; coordinate comparison
        (decremented x10 x9)
        (decremented x9 x8)
        (decremented x8 x7)
        (decremented x7 x6)
        (decremented x6 x5)
        (decremented x5 x4)
        (decremented x4 x3)
        (decremented x3 x2)
        (decremented x2 x1)
        (incremented x1 x2)
        (incremented x2 x3)
        (incremented x3 x4)
        (incremented x4 x5)
        (incremented x5 x6)
        (incremented x6 x7)
        (incremented x7 x8)
        (incremented x8 x9)
        (incremented x9 x10)
        
        (decremented y10 y9)
        (decremented y9 y8)
        (decremented y8 y7)
        (decremented y7 y6)
        (decremented y6 y5)
        (decremented y5 y4)
        (decremented y4 y3)
        (decremented y3 y2)
        (decremented y2 y1)
        (incremented y1 y2)
        (incremented y2 y3)
        (incremented y3 y4)
        (incremented y4 y5)
        (incremented y5 y6)
        (incremented y6 y7)
        (incremented y7 y8)
        (incremented y8 y9)
        (incremented y9 y10)

        ; robots
        (at r1 x1 y1)
        (at r2 x2 y1)
        (at r3 x3 y1)
        (empty r1)
        (empty r2)
        (empty r3)

        ; items
        (item-at A x6 y6)
        (item-at B x5 y8)
        (item-at C x3 y5)
        (item-at D x8 y8)
        (item-at E x5 y8)
        (item-at F x2 y5)
        (item-at G x1 y9)

        ; targets
        (target x10 y10)
        (target x10 y5)
    )

    (:goal (and
        (item-done A)
        (item-done B)
        (item-done C)
        (item-done D)
        (item-done E)
        (item-done F)
        (item-done G)
        )
    )
)