module reg_test();	//testbench


logic rst;
logic clk;
logic wr_en;
logic [4:0] src1_addr;
logic [4:0] src2_addr;
logic [4:0] dest_addr;
logic [31:0] write_data;
logic [31:0] read_data1;
logic [31:0] read_data2;



  dp_reg_file DUT (
    .clk(clk),
    .rst(rst),
	.wr_en(wr_en),
    .src1_addr(src1_addr),
    .src2_addr(src2_addr),
    .dest_addr(dest_addr),
    .write_data(write_data),
	.read_data1(read_data1),
	.read_data2(read_data2)
   );
   
   

// Clock block   
   always begin
   #10; 
   clk=~clk;
   end
   
//Initial block
   initial
   begin
   $dumpfile("dump.vcd"); //dump file
   $dumpvars(0,reg_test); //Output all intermediate signals
   
   clk=1;
   rst=1;
   wr_en=1;
   write_data=10;
   src1_addr=0;
   src2_addr=0;
   dest_addr=0;
   
   
   #20;
   rst=0;
   wr_en=1;
   src1_addr=0;
   src2_addr=3;
   dest_addr=4;
   
   
   #20;
   src2_addr=4;
   
   
   #20;
   src1_addr=5;
   dest_addr=5;
   
   #20;
   src1_addr=5;
   src2_addr=7;
   dest_addr=29;
   
   #20;
   src2_addr=29;
   
   #20;
   rst=1;	//reset asserted again
   
   #20;
   wr_en=0;
   src1_addr=4;
   src2_addr=29;
   dest_addr=8;
   
   
   #20;
   src1_addr=8;
   
   
   $finish;	//finish simulation
   
   end



endmodule