module dp_reg_file( input logic clk,
					input logic rst,
					input logic wr_en,				//write enable
					input logic [4:0] src1_addr,	//read port1
					input logic [4:0] src2_addr,	//read port2
					input logic [4:0] dest_addr,	//write port1
					input logic [31:0] write_data,
					output logic [31:0] read_data1,
					output logic [31:0] read_data2);


logic [31:0] reg_array[0:31];	//register file
logic [31:0] data1;
logic [31:0] data2;


always @(posedge clk)	//write is done sequentially
begin
	if(rst==1'b1) //synchronous reset
	begin
		for(int i=0;i<32;i=i+1)
		begin
			reg_array[i]=32'd0;
		end
	end
	
	else if(wr_en && dest_addr!=5'd0)	//Do not write into zero register
	begin
		reg_array[dest_addr]<=write_data;
	end
	
	else
	begin
		reg_array[dest_addr]<=reg_array[dest_addr];
	end
end

always_comb 	//read is done combinatorially
begin
	if (src1_addr == 5'd0)
		data1 = 32'd0;
	else if ((src1_addr == dest_addr) && wr_en)
		data1 = write_data;
	else
		data1 = reg_array[src1_addr][31:0];
end

always_comb 
begin
	if (src2_addr == 5'd0)	//zero register always reads as zero
		data2 = 32'd0;
	else if ((src2_addr == dest_addr) && wr_en)
		data2 = write_data;
	else
		data2 = reg_array[src2_addr][31:0];
end

assign read_data1=data1;
assign read_data2=data2;



endmodule

 

