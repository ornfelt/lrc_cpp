import sys, os
import math
import pygame
from pygame.locals import *
import time

def main():
    #now = time.time()
    has_slept = False
    has_looped = False
    vels = [float(x)/10000 for x in runners]
    angles = [0 for x in vels]
    speed_fix = 1.5
    pygame.init()
    screen = pygame.display.set_mode((400, 400))
    x = 100
    y = 100

    while True:
      for event in pygame.event.get():
        if event.type == QUIT:
          pygame.quit()
          sys.exit()
        
        if event.type == KEYDOWN:
          if event.key == K_ESCAPE:
            pygame.quit()
            sys.exit()
            
      screen.fill("black")
      
      for i in range(len(vels)):
        if i == 0:
            r = pygame.draw.circle(screen, "green", (x*math.cos(angles[i])+200, y*math.sin(angles[i])+200), 3, 0)
        else:
            r = pygame.draw.circle(screen, "red", (x*math.cos(angles[i])+200, y*math.sin(angles[i])+200), 3, 0)
        
        #print(((angles[0] / math.pi) * 0.5))
        lonely = True
        for j in range(len(vels) - 1):
            d = abs(((angles[j+1] / math.pi) * 0.5) - ((angles[0] / math.pi) * 0.5))
            #print(d)
            d -= int(d)
            #if d < 0.125 or d > 0.875:
                #lonely = False
                #has_slept = False
            if d >= 0.125 and d <= 0.875:
                #print(d)
                pass
            else:
                lonely = False
                has_slept = False
                
        if not has_slept and lonely:
            time.sleep(2)
            has_slept = True
      
        angles[i] += vels[i] * speed_fix

      pygame.display.update()
      
if __name__ == "__main__":
    # OBS! Make sure that input is sorted so that first runner is "runnertocheck"
    print("Arguments in python script: ", sys.argv[1:])
    if ',' in sys.argv[1:][0]:
        runners = sys.argv[1:][0].split(',')
        if "x" not in sys.argv[2:][0]:
            lonely_at = int(sys.argv[2:][0])
            print(lonely_at)
            lonely_at_v = int(sys.argv[3:][0])
            print(lonely_at_v)
        else:
            lonely_at = 500
            lonely_at_v = 1
        
        main()
    sys.exit(os.X_OK) # Code 1, all ok