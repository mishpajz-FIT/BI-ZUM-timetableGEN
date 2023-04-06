;Header and description

(define (domain warehouse)

(:requirements :strips :typing :equality :negative-preconditions)

(:types 
    position 
    item 
    robot
)

(:predicates
    ; robot carrying
    (carrying ?r - robot ?i - item)
    (empty ?r - robot)
    ; coordinates
    (inc ?a ?b - position)
    (dec ?a ?b - position)
    ; robot position
    (at ?r - robot ?x ?y - position)
    ; item position
    (item-at ?i - item ?x ?y - location)
    (target ?x ?y - position)
    ; item parameter
    (item-done ?i)
)

; movement

; move up:
; find x, y, yn such that robot is at position x and y and yn = y - 1,
; then check if there is not another robot at this position (x; yn), if there isn't
; move the robot there
(:action move-up
  :parameters (?r - robot)
  :precondition ()
  :effect (forall (?x ?y ?yn - position)
              (when (and (at ?r ?x ?y)
                         (dec ?y ?yn)
                         (not (exists (?o - robot)
                                 (and (not (= ?r ?o))
                                      (at ?r ?x ?yn)))))
                (and (not (at ?r ?x ?y)) (at ?r ?x ?yn))
              )
          )
)

; move down:
; analogous to move up, only the coordinate is added instead of substracted
(:action move-down
  :parameters (?r - robot)
  :precondition ()
  :effect (forall (?x ?y ?yn - position)
              (when (and (at ?r ?x ?y)
                         (inc ?y ?yn)
                         (not (exists (?o - robot)
                                 (and (not (= ?r ?o))
                                      (at ?r ?x ?yn)))))
                (and (not (at ?r ?x ?y)) (at ?r ?x ?yn))
              )
          )
)

; move left:
; analogous to move up, only the search is performed on x coordiates
(:action move-left
  :parameters (?r - robot)
  :precondition ()
  :effect (forall (?x ?y ?xn - position)
              (when (and (at ?r ?x ?y)
                         (dec ?x ?xn)
                         (not (exists (?o - robot)
                                 (and (not (= ?r ?o))
                                      (at ?r ?xn ?y)))))
                (and (not (at ?r ?x ?y)) (at ?r ?xn ?y))
              )
          )
)

; move up:
; analogous to move down, only the search is performed on x coordiates
(:action move-right
  :parameters (?r - robot)
  :precondition ()
  :effect (forall (?x ?y ?xn - position)
              (when (and (at ?r ?x ?y)
                         (inc ?x ?xn)
                         (not (exists (?o - robot)
                                 (and (not (= ?r ?o))
                                      (at ?r ?xn ?y)))))
                (and (not (at ?r ?x ?y)) (at ?r ?xn ?y))
              )
          )
)

; pick item from ground (cant be completed)
(:action pick-up
    :parameters (?r - robot ?i - item ?x ?y - location)
    :precondition (and (at ?r ?x ?y) (item-at ?i ?x ?y) (empty ?r) (not (item-done ?i)))
    :effect (and (not (item-at ?i ?x ?y)) (not (empty ?r)) (carrying ?r ?i))
)

; put item to ground (not at goal position)
(:action put-down
    :parameters (?r - robot ?i - item ?x ?y - location)
    :precondition (and (at ?r ?x ?y) (carrying ?r ?i) (not (target ?x ?y)))
    :effect (and (not (carrying ?r ?i)) (empty ?r) (item-at ?x ?y))
)

; put item at goal position
(:action put-goal
    :parameters (?r - robot ?i - item ?x ?y - location)
    :precondition (and (at ?r ?x ?y) (carrying ?r ?i) (target ?x ?y))
    :effect (and (not (carrying ?r ?i)) (empty ?r) (item-at ?x ?y) (item-done ?i))
)

)