
module fifo
#(
parameter fifo_depth = 4,
parameter fifo_width = 32
)
(
input clk,
input reset,
input push,
input pop,
input [fifo_width-1:0] data_in,
output reg fifo_full,
output reg fifo_empty,
output reg [fifo_width-1:0] data_out
);

    reg [fifo_width-1:0] myfifo [fifo_depth-1:0];
   
    reg    [fifo_depth-1:0] wr_ptr;
reg    [fifo_depth-1:0] rd_ptr;

reg          bit_comp;

reg [fifo_width-1:0] register1;
reg [fifo_width-1:0] register2;
reg [fifo_width-1:0] register3;
reg [fifo_width-1:0] register4;

reg                    d_mux_out1;
reg                    d_mux_out2;
reg                    d_mux_out3;
reg                    d_mux_out4;

wire                    clr_wr;
wire                    clr_rd;

always@(*) begin
  case(rd_ptr)
      4'd0 : data_out = register1;
      4'd1 : data_out = register2;
      4'd2 : data_out = register3;
      4'd3 : data_out = register4;
  endcase
end

always@(*) begin
  case(wr_ptr)
      4'd0 : {d_mux_out1,d_mux_out2,d_mux_out3,d_mux_out4} = {push, 3'd0       };
      4'd1 : {d_mux_out1,d_mux_out2,d_mux_out3,d_mux_out4} = {1'd0,push, 2'd0  };
      4'd2 : {d_mux_out1,d_mux_out2,d_mux_out3,d_mux_out4} = {2'd0,push, 1'd0  };
      4'd3 : {d_mux_out1,d_mux_out2,d_mux_out3,d_mux_out4} = {3'd0,push        };
  endcase
end


always@(posedge clk) begin
if(reset) begin
register1 <= 0;
end
else if(d_mux_out1) begin
 register1 <= data_in;
end
end

always@(posedge clk) begin
if(reset) begin
register2 <= 0;
end
else if(d_mux_out2) begin
 register2 <= data_in;
end
end

always@(posedge clk) begin
if(reset) begin
register3 <= 0;
end
else if(d_mux_out3) begin
 register3 <= data_in;
end
end

always@(posedge clk) begin
if(reset) begin
register4 <= 0;
end
else if(d_mux_out4) begin
 register4 <= data_in;
end
end

always@(posedge clk) begin
if(reset) begin
wr_ptr <= 0;
end
else if(push & !fifo_full) begin
 wr_ptr <= wr_ptr + 1;
end
end

always@(posedge clk) begin
if(reset) begin
rd_ptr   <= 0;
end
else if(pop & !fifo_empty) begin
 rd_ptr <= rd_ptr + 1;
end
end

always@(*) begin
  bit_comp = wr_ptr[2] ^ rd_ptr[2];
end

    always@(*) begin
    fifo_full = bit_comp & (wr_ptr[1:0] == rd_ptr[1:0]);
    fifo_empty = ~bit_comp & (wr_ptr[1:0] == rd_ptr[1:0]);
    end

endmodule

