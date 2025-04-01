import turtle
import math
import random

# Function to calculate distance between two points
def distance(p1, p2):
    return math.sqrt((p1[0] - p2[0])**2 + (p1[1] - p2[1])**2)

# Function to get the closest goal for a robot
def closest_goal(robot_position, goals):
    closest_distance = float('inf')
    closest_goal = None
    for goal in goals:
        dist = distance(robot_position, goal)
        if dist < closest_distance:
            closest_distance = dist
            closest_goal = goal
    return closest_goal

# Function to move a robot to a goal (using turtle graphics for visualization)
def move_to_goal(robot, goal):
    robot.setheading(robot.towards(goal))  # Point robot towards the goal
    while distance(robot.pos(), goal) > 10:  # Continue moving until close enough
        robot.forward(1)  # Move forward by 1 unit

# Setup screen for visualization
screen = turtle.Screen()
screen.setup(width=800, height=600)
screen.bgcolor("white")

# Create two "TurtleBot" turtles
turtlebot1 = turtle.Turtle()
turtlebot2 = turtle.Turtle()

# Set the initial positions for both robots
turtlebot1.penup()
turtlebot1.setpos(-300, 100)  # TurtleBot1 starts at (-300, 100)
turtlebot1.pendown()

turtlebot2.penup()
turtlebot2.setpos(-300, -100)  # TurtleBot2 starts at (-300, -100)
turtlebot2.pendown()

# Label the robots
turtlebot1_label = turtle.Turtle()
turtlebot1_label.penup()
turtlebot1_label.setpos(turtlebot1.pos()[0], turtlebot1.pos()[1] + 20)
turtlebot1_label.write("TurtleBot 1", align="center", font=("Arial", 12, "normal"))
turtlebot1_label.hideturtle()

turtlebot2_label = turtle.Turtle()
turtlebot2_label.penup()
turtlebot2_label.setpos(turtlebot2.pos()[0], turtlebot2.pos()[1] - 20)
turtlebot2_label.write("TurtleBot 2", align="center", font=("Arial", 12, "normal"))
turtlebot2_label.hideturtle()

# Example goal points (x, y)
bounding_value_positive = 150
bounding_value_negative = -150
goal_points = [
    (random.randint(bounding_value_negative, bounding_value_positive), random.randint(bounding_value_negative, bounding_value_positive)),
    (random.randint(bounding_value_negative, bounding_value_positive), random.randint(bounding_value_negative, bounding_value_positive)),
    (random.randint(bounding_value_negative, bounding_value_positive), random.randint(bounding_value_negative, bounding_value_positive)),
    (random.randint(bounding_value_negative, bounding_value_positive), random.randint(bounding_value_negative, bounding_value_positive))
]

# Draw the goal points and label them
goal_labels = []
for i, goal in enumerate(goal_points):
    goal_marker = turtle.Turtle()
    goal_marker.penup()
    goal_marker.setpos(goal)
    goal_marker.shape("circle")
    goal_marker.fillcolor("red")
    goal_marker.stamp()

    goal_label = turtle.Turtle()
    goal_label.penup()
    goal_label.setpos(goal[0], goal[1] + 10)  # Label above the goal point
    goal_label.write(f"Goal {i + 1}", align="center", font=("Arial", 10, "normal"))
    goal_label.hideturtle()
    goal_labels.append(goal_label)

# Main loop for moving both robots
remaining_goals = goal_points.copy()

while remaining_goals:
    # Get the current positions of both robots
    robot1_position = turtlebot1.pos()
    robot2_position = turtlebot2.pos()

    # Find the closest goal for TurtleBot 1 and remove that goal from the remaining list
    goal1 = closest_goal(robot1_position, remaining_goals)
    print(f'Moving TurtleBot 1 to goal {goal1}')
    move_to_goal(turtlebot1, goal1)
    
    # Remove goal1 from the remaining goals list after TurtleBot 1 moves
    remaining_goals.remove(goal1)

    # Find the closest goal for TurtleBot 2 from the updated remaining goals
    goal2 = closest_goal(robot2_position, remaining_goals)
    print(f'Moving TurtleBot 2 to goal {goal2}')
    move_to_goal(turtlebot2, goal2)

    # Remove goal2 from the remaining goals list after TurtleBot 2 moves
    remaining_goals.remove(goal2)

# Keep the window open until clicked
screen.exitonclick()
