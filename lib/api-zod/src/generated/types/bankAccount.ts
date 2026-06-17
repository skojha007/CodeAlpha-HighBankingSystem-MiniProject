import * as zod from "zod";

/**
 * Bank account details
 */
export const BankAccountResponse = zod.object({
  id: zod.number(),
  accountNumber: zod.string(),
  holderName: zod.string(),
  balance: zod.string(), // decimal as string for precision
  createdAt: zod.string().datetime(),
});

/**
 * Request to create a new bank account
 */
export const CreateAccountRequest = zod.object({
  accountNumber: zod.string().min(1, "Account number is required"),
  holderName: zod.string().min(1, "Holder name is required"),
  initialDeposit: zod.string().min("0", "Initial deposit must be positive"),
  pin: zod.string().length(4, "PIN must be exactly 4 digits"),
});

/**
 * Deposit/Withdrawal request
 */
export const TransactionRequest = zod.object({
  accountNumber: zod.string(),
  amount: zod.string(),
  pin: zod.string(),
});

/**
 * Transfer request
 */
export const TransferRequest = zod.object({
  sourceAccountNumber: zod.string(),
  destinationAccountNumber: zod.string(),
  amount: zod.string(),
  pin: zod.string(),
});

export type BankAccountResponse = zod.infer<typeof BankAccountResponse>;
export type CreateAccountRequest = zod.infer<typeof CreateAccountRequest>;
export type TransactionRequest = zod.infer<typeof TransactionRequest>;
export type TransferRequest = zod.infer<typeof TransferRequest>;
