# Setup & Installation Guide

## Quick Start

### 1. Prerequisites

- **Node.js** 20 or higher ([Download](https://nodejs.org))
- **pnpm** 9 or higher (`npm install -g pnpm`)
- **PostgreSQL** 14 or higher (for database)

### 2. Clone Repository

```bash
git clone https://github.com/skojha007/CodeAlpha-HighBankingSystem-MiniProject.git
cd CodeAlpha-HighBankingSystem-MiniProject
```

### 3. Environment Configuration

Copy the example environment file:

```bash
cp .env.example .env
```

Edit `.env` with your configuration:

```bash
# Server
NODE_ENV=development
PORT=3000
LOG_LEVEL=info

# Database (local development)
DATABASE_URL=postgresql://postgres:password@localhost:5432/bank_system
```

### 4. Database Setup (Local Development)

#### Option A: Using Docker (Recommended)

```bash
# Start PostgreSQL container
docker run -d \
  --name postgres-bank \
  -e POSTGRES_USER=postgres \
  -e POSTGRES_PASSWORD=password \
  -e POSTGRES_DB=bank_system \
  -p 5432:5432 \
  postgres:16-alpine

# Verify connection
docker exec postgres-bank psql -U postgres -d bank_system -c "SELECT 1"
```

#### Option B: Using Docker Compose

```bash
# Create docker-compose.yml with PostgreSQL service
docker-compose up -d

# Wait for database to be ready
```

#### Option C: Manual PostgreSQL Installation

```bash
# macOS (Homebrew)
brew install postgresql@16
brew services start postgresql@16

# Linux (Ubuntu/Debian)
sudo apt-get install postgresql postgresql-contrib
sudo systemctl start postgresql

# Windows
# Download from https://www.postgresql.org/download/windows/
```

### 5. Install Dependencies

```bash
# Install all workspace dependencies
pnpm install

# Verify installation
pnpm --version
node --version
```

### 6. Run Database Migrations

```bash
# Create database schema
pnpm migrate

# Output should show: "Pushed successfully"
```

### 7. Build Project

```bash
# Type check and build
pnpm run build

# Output: build complete
```

### 8. Start Development Server

```bash
# Start the API server
pnpm run dev

# Server should start on http://localhost:3000
# You'll see: "Server listening on port 3000"
```

### 9. Verify Setup

```bash
# Test health endpoint in another terminal
curl http://localhost:3000/api/healthz

# Expected response:
# {"status":"ok"}
```

## Project Structure

```
.
├── artifacts/
│   └── api-server/          # Express API server
│       ├── src/
│       │   ├── app.ts       # Express app setup
│       │   ├── index.ts     # Server entry point
│       │   ├── routes/      # API route handlers
│       │   ├── lib/         # Utilities (logger, helpers)
│       │   └── middlewares/ # Express middlewares
│       ├── build.mjs        # ESBuild configuration
│       └── package.json
├── lib/
│   ├── db/                  # Database layer
│   │   ├── src/
│   │   │   ├── index.ts     # Database connection
│   │   │   └── schema/      # Drizzle ORM schemas
│   │   └── drizzle.config.ts
│   ├── api-zod/             # API validation schemas
│   │   └── src/generated/   # Generated Zod schemas
│   └── api-client-react/    # React API client (future)
├── scripts/                 # Build scripts
├── pnpm-workspace.yaml      # Workspace configuration
├── package.json             # Root package
├── tsconfig.json            # TypeScript config
├── .env.example             # Environment template
├── SETUP.md                 # This file
└── DEPLOYMENT.md            # Deployment guide
```

## Development

### Common Commands

```bash
# Install new package in specific workspace
pnpm add express --filter "@workspace/api-server"

# Run tests
pnpm test

# Type checking
pnpm run typecheck

# Build everything
pnpm run build

# Start in development mode
pnpm run dev

# Database commands
pnpm -w run -r --filter "@workspace/db" run push      # Apply migrations
pnpm -w run -r --filter "@workspace/db" run generate  # Generate new migration
```

### Adding Dependencies

```bash
# Add to API server
pnpm add bcryptjs --filter "@workspace/api-server"

# Add dev dependency
pnpm add --save-dev @types/node --filter "@workspace/api-server"

# Add to all workspaces
pnpm add -r zod
```

## Troubleshooting

### "ECONNREFUSED - PostgreSQL not found"

```bash
# Start PostgreSQL
docker start postgres-bank

# Or if not created:
docker run -d --name postgres-bank -e POSTGRES_PASSWORD=password -p 5432:5432 postgres:16-alpine
```

### "DATABASE_URL is not set"

```bash
# Set environment variable
export DATABASE_URL=postgresql://postgres:password@localhost:5432/bank_system

# Or add to .env
echo "DATABASE_URL=postgresql://postgres:password@localhost:5432/bank_system" >> .env
```

### "pnpm: command not found"

```bash
# Install pnpm globally
npm install -g pnpm

# Verify
pnpm --version
```

### Build or Type Check Fails

```bash
# Clean install
rm -rf node_modules .pnpm-store
pnpm install

# Rebuild
pnpm run build

# Check for TypeScript errors
pnpm run typecheck
```

### Port 3000 Already in Use

```bash
# Find process using port 3000 (Linux/macOS)
lsof -i :3000

# Kill process
kill -9 <PID>

# Or use different port
PORT=3001 pnpm run dev
```

## Next Steps

1. **Read API Documentation** - See `DEPLOYMENT.md` for API endpoints
2. **Explore Code** - Start with `artifacts/api-server/src/routes/health.ts`
3. **Add Features** - Create new routes and database schemas
4. **Deploy** - Follow `DEPLOYMENT.md` for production deployment

## Resources

- **TypeScript**: https://www.typescriptlang.org/docs
- **Express.js**: https://expressjs.com
- **Drizzle ORM**: https://orm.drizzle.team
- **PostgreSQL**: https://www.postgresql.org/docs
- **Zod**: https://zod.dev

## Support

For issues or questions:
1. Check `.env` configuration
2. Verify PostgreSQL is running: `docker ps`
3. Check logs: `docker logs postgres-bank`
4. Review error messages carefully
