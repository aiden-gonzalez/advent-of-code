import re

class Position:
  def __init__(self, x, y):
    self.x = x
    self.y = y
  
  def __str__(self):
    return "(" + str(self.x) + ", " + str(self.y) + ")"

class Button:
  def __init__(self, x_diff=0, y_diff=0):
    self.x_diff = x_diff
    self.y_diff = y_diff

  def __str__(self):
    return "(X+" + str(self.x_diff) + ", Y+" + str(self.y_diff) + ")"
  
class Solution:
  def __init__(self, a_presses, b_presses):
    self.a_presses = a_presses
    self.b_presses = b_presses

  def __str__(self):
    return "(AP " + str(self.a_presses) + " BP " + str(self.b_presses) + ")"

  def cost(self):
    return (3 * self.a_presses) + self.b_presses

class Machine:
  def __init__(self, a_button, b_button, prize_position=Position(0,0)):
    self.a_button = a_button
    self.b_button = b_button
    self.prize_position = prize_position
    self.solution = None

  def __str__(self):
    return "Machine: A" + str(self.a_button) + " B" + str(self.b_button) + " P" + str(self.prize_position) + " S" + str(self.solution)


def extract_diffs(diff_line):
  m = re.search('Button [A-B]: X\+([0-9]+), Y\+([0-9]+)', diff_line)
  if m:
    return (int(m.group(1)), int(m.group(2)))
  else:
    return None

BIG_NUMBER = 10000000000000

def extract_position(position_line):
  m = re.search('Prize: X=([0-9]+), Y=([0-9]+)', position_line)
  if m:
    # return Position(int(m.group(1)), int(m.group(2)))
    # For part 2
    return Position(int(m.group(1)) + BIG_NUMBER, int(m.group(2)) + BIG_NUMBER)
  else:
    return None

machines = []
with open("input.txt", "r") as textfile:
  text = textfile.readlines()
  a_button = Button()
  b_button = Button()
  machine = Machine(a_button, b_button)
  for line in text:
    if line.startswith("Button A"):
      diffs = extract_diffs(line)
      a_button.x_diff = diffs[0]
      a_button.y_diff = diffs[1]
    elif line.startswith("Button B"):
      diffs = extract_diffs(line)
      b_button.x_diff = diffs[0]
      b_button.y_diff = diffs[1]
    elif line.startswith("Prize"):
      machine.prize_position = extract_position(line)
      machines.append(machine)
      a_button = Button()
      b_button = Button()
      machine = Machine(a_button, b_button)

# Now for each machine, solve the system of equations
for machine in machines:
  p = machine.prize_position
  a = machine.a_button
  b = machine.b_button
  # Test for infinite solutions case
  bp_numerator = p.x - (a.x_diff * (p.y / a.y_diff))
  bp_denominator = b.x_diff - (a.x_diff * (b.y_diff / a.y_diff))
  # Part 2 addition here
  # bp = (bp_numerator + BIG_NUMBER * (1 - (a.x_diff / a.y_diff))) / bp_denominator
  bp = bp_numerator / bp_denominator
  # If bp is positive and a whole number, this could be a solution
  if not (bp >= 0 and abs(round(bp) - bp) < 0.001):
    continue
  # Find ap
  ap = (p.y / a.y_diff) - ((b.y_diff / a.y_diff) * bp)
  # If ap is positive and a whole number, this is a solution
  if not (ap >= 0 and abs(round(ap) - ap) < 0.001):
    continue
  # Can't press a button more than 100 times (removed for part 2)
  #if (ap > 100 or bp > 100):
  #  continue
  machine.solution = Solution(ap, bp)

total_cost = 0
for machine in machines:
  print(machine)
  if machine.solution is not None:
    total_cost += machine.solution.cost()

print(round(total_cost))

# TODO what happens when there are infinitely many solutions (one equation is a multiple of the other)
