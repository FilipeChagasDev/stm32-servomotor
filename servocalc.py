def head():
    print("=============")
    print("TIMER REGISTERS CALCULATOR FOR STM32")
    print("This application is for calculating the PSC and ARR registers of the PWM generation timer for servomotor")
    print("by Filipe Chagas")
    print("v1.0.0")
    print("=============")

def main():
    apb_freq = int(input("type the APBx timer clock frequency in MHZ: "))

    psc_arr = int((apb_freq * 1000000) / 50)

    print("Prescaler*Period = " + str(psc_arr))

    arr = 0xFFFF
    psc = 1
    
    while arr >= 1:

        psc = 1
        while psc <= 0xFFFF:

            if psc * arr == psc_arr:
                print("\n--- timer registers ---")
                print("Prescaler: " + str(psc))
                print("Counter Period (AutoReload Register): " + str(arr))
                return

            if psc * arr > psc_arr:
                break

            psc += 1
        arr -= 1
    
    print("Error")

head()
main()