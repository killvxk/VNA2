--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   21:25:23 09/18/2020
-- Design Name:   
-- Module Name:   /home/jan/Projekte/VNA2/FPGA/VNA/Test_DFT.vhd
-- Project Name:  VNA
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: DFT
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY Test_DFT IS
END Test_DFT;
 
ARCHITECTURE behavior OF Test_DFT IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT DFT
	 GENERIC(BINS : integer);
    PORT(
         CLK : IN  std_logic;
         RESET : IN  std_logic;
         PORT1 : IN  std_logic_vector(15 downto 0);
         PORT2 : IN  std_logic_vector(15 downto 0);
         NEW_SAMPLE : IN  std_logic;
         BIN1_PHASEINC : IN  std_logic_vector(15 downto 0);
         DIFFBIN_PHASEINC : IN  std_logic_vector(15 downto 0);
         WINDOW_INC : IN  std_logic_vector(15 downto 0);
         WINDOW_TYPE : IN  std_logic_vector(1 downto 0);
         RESULT_READY : OUT  std_logic;
         OUTPUT : out  STD_LOGIC_VECTOR (191 downto 0);
         NEXT_OUTPUT : IN  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal CLK : std_logic := '0';
   signal RESET : std_logic := '0';
   signal PORT1 : std_logic_vector(15 downto 0) := (others => '0');
   signal PORT2 : std_logic_vector(15 downto 0) := (others => '0');
   signal NEW_SAMPLE : std_logic := '0';
   signal BIN1_PHASEINC : std_logic_vector(15 downto 0) := (others => '0');
   signal DIFFBIN_PHASEINC : std_logic_vector(15 downto 0) := (others => '0');
   signal WINDOW_INC : std_logic_vector(15 downto 0) := (others => '0');
   signal WINDOW_TYPE : std_logic_vector(1 downto 0) := (others => '0');
   signal NEXT_OUTPUT : std_logic := '0';

 	--Outputs
   signal RESULT_READY : std_logic;
	signal OUTPUT : STD_LOGIC_VECTOR (191 downto 0);

   -- Clock period definitions
   constant CLK_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: DFT
	GENERIC MAP(BINS => 100)
	PORT MAP (
          CLK => CLK,
          RESET => RESET,
          PORT1 => PORT1,
          PORT2 => PORT2,
          NEW_SAMPLE => NEW_SAMPLE,
          BIN1_PHASEINC => BIN1_PHASEINC,
          DIFFBIN_PHASEINC => DIFFBIN_PHASEINC,
          WINDOW_INC => WINDOW_INC,
          WINDOW_TYPE => WINDOW_TYPE,
          RESULT_READY => RESULT_READY,
          OUTPUT => OUTPUT,
          NEXT_OUTPUT => NEXT_OUTPUT
        );

   -- Clock process definitions
   CLK_process :process
   begin
		CLK <= '0';
		wait for CLK_period/2;
		CLK <= '1';
		wait for CLK_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
		RESET <= '1';
		PORT1 <= "1000000000000000";
		PORT2 <= "0100000000000000";
		BIN1_PHASEINC <= "0100000000000000";
		DIFFBIN_PHASEINC <= "0010000000000000";
		WINDOW_INC <= "0000100000000000";
      wait for 100 ns;	
		RESET <= '0';
      wait for CLK_period*10;
		NEW_SAMPLE <= '1';
		wait for CLK_period;
		NEW_SAMPLE <= '0';
		--wait until RESULT_READY = '1';
		wait for CLK_period*112;
		NEW_SAMPLE <= '1';
		wait for CLK_period;
		NEW_SAMPLE <= '0';
      -- insert stimulus here 

      wait;
   end process;

END;
