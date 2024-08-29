module fifo
#(
    parameter int fifo_depth = 4,
    parameter int fifo_width = 32
)
(
    input logic                    clk,
    input logic                    reset,
    input logic                    push,
    input logic                    pop,
    input logic [fifo_width-1:0]   data_in,
    output logic                   fifo_full,
    output logic                   fifo_empty,
    output logic [fifo_width-1:0]  data_out
);

    logic [fifo_width-1:0] myfifo [fifo_depth-1:0];
   
    logic [fifo_depth-1:0] wr_ptr;
    logic [fifo_depth-1:0] rd_ptr;
   
    logic bit_comp;
   
    logic [fifo_width-1:0] register1;
    logic [fifo_width-1:0] register2;
    logic [fifo_width-1:0] register3;
    logic [fifo_width-1:0] register4;
   
    logic d_mux_out1;
    logic d_mux_out2;
    logic d_mux_out3;
    logic d_mux_out4;
   
    logic clr_wr;
    logic clr_rd;
   
    always_comb begin
        case (rd_ptr)
            4'd0: data_out = register1;
            4'd1: data_out = register2;
            4'd2: data_out = register3;
            4'd3: data_out = register4;
            default: data_out = '0; // Default case added for completeness
        endcase
    end

   
    always_comb begin
        case (wr_ptr)
            4'd0: {d_mux_out1, d_mux_out2, d_mux_out3, d_mux_out4} = {push, 3'b0};
            4'd1: {d_mux_out1, d_mux_out2, d_mux_out3, d_mux_out4} = {1'b0, push, 2'b0};
            4'd2: {d_mux_out1, d_mux_out2, d_mux_out3, d_mux_out4} = {2'b0, push, 1'b0};
            4'd3: {d_mux_out1, d_mux_out2, d_mux_out3, d_mux_out4} = {3'b0, push};
            default: {d_mux_out1, d_mux_out2, d_mux_out3, d_mux_out4} = 4'b0; // Default case added
        endcase
    end
   
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            register1 <= '0;
        end else if (d_mux_out1) begin
            register1 <= data_in;
        end
    end
   
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            register2 <= '0;
        end else if (d_mux_out2) begin
            register2 <= data_in;
        end
    end
   
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            register3 <= '0;
        end else if (d_mux_out3) begin
            register3 <= data_in;
        end
    end
   
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            register4 <= '0;
        end else if (d_mux_out4) begin
            register4 <= data_in;
        end
    end
   
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            wr_ptr <= '0;
        end else if (push && !fifo_full) begin
            wr_ptr <= wr_ptr + 1;
        end
    end
   
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            rd_ptr <= '0;
        end else if (pop && !fifo_empty) begin
            rd_ptr <= rd_ptr + 1;
        end    
    end
   
    always_comb begin
        bit_comp = wr_ptr[2] ^ rd_ptr[2];
    end
   
    always_comb begin
        fifo_full = bit_comp && (wr_ptr[1:0] == rd_ptr[1:0]);
        fifo_empty = !bit_comp && (wr_ptr[1:0] == rd_ptr[1:0]);
    end
   
endmodule
